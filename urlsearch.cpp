/**
 * PasswordMaker - Creates and manages passwords
 * Copyright (C) 2005 Eric H. Jung and LeahScape, Inc.
 * http://passwordmaker.org/
 * grimholtz@yahoo.com
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESSFOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Written by Miquel Burns <miquelfire@gmail.com> and Eric H. Jung
 * urlsearch.cpp by Dave Marotti <landshark666@gmail.com>
 */
#include <string>
#include <iostream>
#include <sstream>
#include <pcrecpp.h>
#include "stdafx.h"
#include "tinyxml.h"
#include "urlsearch.h"

using namespace std;

int UrlSearch::regexCmp(const char *regexp, const char *string) {
	pcrecpp::RE re(regexp);

	if(re.FullMatch(string)) {
		return 1;
	}
	else {
		return 0;
	}
}

int UrlSearch::wildcardCmp(const char *wild, const char *string) {
  // Written by Jack Handy - jakkhandy@hotmail.com
  // Found at: http://www.codeproject.com/KB/string/wildcmp.aspx
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
	 if ((*wild != *string) && (*wild != '?')) {
		return 0;
	 }
	 wild++;
	 string++;
  }

  while (*string) {
	 if (*wild == '*') {
		if (!*++wild) {
		  return 1;
		}
		mp = wild;
		cp = string+1;
	 }
    else if ((*wild == *string) || (*wild == '?')) {
		wild++;
		string++;
	 }
    else {
		wild = mp;
		string = cp++;
	 }
  }

  while (*wild == '*') {
	 wild++;
  }
  return !*wild;
}

bool UrlSearch::search(string url, string filepath)
{
	TiXmlDocument document(filepath);
	document.LoadFile();
	string ns;
	int attrnum;

	if (!document.Error()) {
		TiXmlHandle docHandle(&document);
		TiXmlElement *defaultNode = docHandle.FirstChildElement("RDF:RDF").Element();

		if (defaultNode) {
			TiXmlAttribute *attrib = defaultNode->FirstAttribute();
			// Find the namespace
			while (attrib) {
				if (!strcmp(attrib->Value(), "http://passwordmaker.mozdev.org/rdf#")) {
					ns = attrib->Name();
					ns.erase(ns.begin(), ns.begin() + (ns.find(':') + 1));
					break;
				}
				attrib = attrib->Next();
			}
			if (ns.empty()) {
				// Namespace was not found. Not a valid file?
			}
         else {
				defaultNode = docHandle.FirstChildElement("RDF:RDF").FirstChildElement("RDF:Description").Element();
				while (defaultNode) {
					if(defaultNode->Attribute(ns+":name")) {
						// iterate through all possible patterns
						attrnum = 0;
						while(1) {
							ostringstream attrname, attrtype, attrenabled;
							attrname << ns << ":pattern" << attrnum;
							attrtype << ns << ":patterntype" << attrnum;
							attrenabled << ns << ":patternenabled" << attrnum;

							// Does this node have a pattern attribute?
							if(defaultNode->Attribute(attrname.str())) {
								string pattern = string(defaultNode->Attribute(attrname.str()));
								string type = string(defaultNode->Attribute(attrtype.str()));
								string enabled = string(defaultNode->Attribute(attrenabled.str()));

								// Locate it based on regexp/wildcard patterns
								bool found = false;
								if(type=="wildcard")
									found = wildcardCmp(pattern.c_str(), url.c_str());
								else if(type=="regex")
									found = regexCmp(pattern.c_str(), url.c_str());

								if(found) {
									name = defaultNode->Attribute(ns+":name");
									description = defaultNode->Attribute(ns+":description");
									username = defaultNode->Attribute(ns+":usernameTB");
									return true;
								}

								attrnum++;
							}
							else {
								break;
							}
						}
					} // end has ns1:name
					defaultNode = defaultNode->NextSiblingElement("RDF:Description");
				}
			}
		} // end if(defaultNode)
	} // end if(!document.Error())

	return false;
}

UrlSearch::UrlSearch() {
	name = "";
	description = "";
	username = "";
}

UrlSearch::~UrlSearch() {
}

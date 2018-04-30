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
 * listaccounts.cpp by Dave Marotti <landshark666@gmail.com>
 */
#include <vector>
#include <algorithm>
#include <set>
#include "stdafx.h"
#include "tinyxml.h"
#include "listaccounts.h"

using namespace std;

static set<string> findSeq(string filepath, string about);
static string findDesc(string filepath, string about);


// Not the fastest nor most elegant method of parsing this thing, but it
// works. Well, it works for my password file. -dave
void listAccounts(string filepath, string about, int indent) {
	set<string> list;
	set<string>::iterator iter;

	list = findSeq(filepath, about);

	if(list.size()>0) {
		for(iter = list.begin(); iter!=list.end(); iter++) {
			set<string> sublist = findSeq(filepath, *iter);

			cout << string(indent, ' ') << findDesc(filepath, *iter) << endl;
			for(set<string>::iterator i2=sublist.begin(); i2!=sublist.end(); i2++) {
				listAccounts(filepath, *i2, indent+4);
			}
		}
	}
	else {
		cout << string(indent, ' ') << findDesc(filepath, about) << endl;
	}
}


// Gets the set of subnodes for a given RDF:Seq node
set<string> findSeq(string filepath, string about) {
	TiXmlDocument document(filepath);
	document.LoadFile();
	string ns;
	set<string> list;

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
				defaultNode = NULL;
			}
         else {
				defaultNode = docHandle.FirstChildElement("RDF:RDF").FirstChildElement("RDF:Seq").Element();
				while (defaultNode) {
					if(!strcmp(defaultNode->Attribute("RDF:about"), about.c_str())) {
						// dump all children
						TiXmlElement *liNode = defaultNode->FirstChildElement("RDF:li");
						while(liNode) {
							const char *res = liNode->Attribute("RDF:resource");
							if(res) {
								list.insert(string(res));
							}
							liNode = liNode->NextSiblingElement("RDF:li");
						}
						return list;
					}
					defaultNode = defaultNode->NextSiblingElement("RDF:Seq");
				}
			}
		} // end if(defaultNode)
	} // end if(!document.Error())

	return list;
}

// Returns the account name for a description node
string findDesc(string filepath, string about) {
	TiXmlDocument document(filepath);
	document.LoadFile();
	string ns;

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
				defaultNode = NULL;
			}
         else {
				defaultNode = docHandle.FirstChildElement("RDF:RDF").FirstChildElement("RDF:Description").Element();
				while (defaultNode) {
					if(!strcmp(defaultNode->Attribute("RDF:about"), about.c_str())) {
						return string(defaultNode->Attribute(ns+":name"));
					}
					defaultNode = defaultNode->NextSiblingElement("RDF:Description");
				}
			}
		} // end if(defaultNode)
	} // end if(!document.Error())

	return string("UNKNOWN");
}

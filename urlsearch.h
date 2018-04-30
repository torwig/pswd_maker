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
 * urlsearch.h by Dave Marotti <landshark666@gmail.com>
 */
#ifndef URLSEARCH_H
#define URLSEARCH_H

#include <string>

class UrlSearch {
	private:
		std::string name;
		std::string description;
		std::string username;

	public:
		UrlSearch();
		virtual ~UrlSearch();

		bool search(std::string url, std::string filepath);

		inline std::string getName() { return name; }
		inline std::string getDescription() { return description; }
		inline std::string getUsername() { return username; }

	private:
		int regexCmp(const char *regexp, const char *string);
		int wildcardCmp(const char *wild, const char *string);
};

#endif

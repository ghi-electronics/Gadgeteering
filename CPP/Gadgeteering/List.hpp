/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _LIST_H_
#define _LIST_H_

namespace GHI
{
	class List
	{
		struct ListNode {
            void* data;
			ListNode* next;
			ListNode* prev;
		};

		ListNode* head;
		ListNode* tail;
		ListNode* currentIteration;

		unsigned int count;

		public:
			List();
			~List();

			bool ended();
            unsigned int getSize() const;

            void add(void *data);
            void remove(void *data);
            bool contains(void *data);
            void *start();
            void *next();
            void push(void *data);
            void *pop();

			void addV(void* data);
			void removeV(void* data);
			bool containsV(void* data);
			void* startV();
			void* nextV();
			void pushV(void* data);
			void* popV();
	};
}

#endif

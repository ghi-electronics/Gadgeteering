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

		public:
			List();
			~List();
			void add(void* data);
			void remove(void* data);
			bool contains(void* data);
			void* start();
			void* next();
			bool ended();
	};
}

#endif
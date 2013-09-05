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

#include "List.hpp"
#include "Types.hpp"

using namespace GHI;

List::List() {
	this->head = NULL;
	this->tail = NULL;
	this->count = 0;
}

List::~List() {
	ListNode* current = this->head;
	ListNode* next;

	while (current) {
		next = current->next;

		delete current;
		
		current = next;
	}
}

void List::add(void* data) {
	this->count++;

	if (this->head == NULL) {
		this->head = new ListNode;
		this->head->next = NULL;
		this->head->prev = NULL;
		this->head->data = data;

		this->tail = this->head;

		return;
	}

	ListNode* newNode = new ListNode;
	newNode->next = NULL;
	newNode->prev = this->tail;
	newNode->data = data;

	this->tail->next = newNode;

	this->tail = newNode;
}

void List::remove(void* data) {
	if (this->head && this->head->data == data) {
		ListNode* newHead = this->head->next;
		delete this->head;
		this->head = newHead;
		if (this->head)
			this->head->prev = NULL;
		count--;
		return;
	}

	if (this->tail && this->tail->data == data) {
		ListNode* newTail = this->tail->prev;
		delete this->tail;
		this->tail = newTail;
		if (this->tail)
			this->tail->next = NULL;
		count--;
		return;
	}

	ListNode* current = this->head->next;
	while (current) {
		if (current->data == data) {
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
			count--;
			return;
		}
			
		current = current->next;
	}
}

bool List::contains(void* data) {
	ListNode* current = this->head;

	while (current) {
		if (current->data == data)
			return true;
		
		current = current->next;
	}

	return false;

}

void* List::start() {
	this->currentIteration = this->head;

	if (!this->head)
		return NULL;

	return this->currentIteration->data;
}

void* List::next() {
	this->currentIteration = this->currentIteration->next;
	
	return this->currentIteration ? this->currentIteration->data : NULL;
}

bool List::ended() {
	return this->currentIteration == NULL;
}

void List::push(void* data) {
	this->add(data);
}

void* List::pop() {
	if (this->count == 0)
		return NULL;

	void* data = this->head->data;

	ListNode* newHead = this->head->next;
	delete this->head;
	this->head = newHead;
	this->head->prev = NULL;
	count--;

	return data;
}

unsigned int List::getSize() const {
	return this->count;
}
#include "List.hpp"
#include "Types.hpp"

using namespace GHI;

List::List() {
	this->head = NULL;
	this->tail = NULL;
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
		this->head->prev = NULL;

		return;
	}

	if (this->tail && this->tail->data == data) {
		ListNode* newTail = this->tail->prev;
		delete this->tail;
		this->tail = newTail;
		this->tail->next = NULL;

		return;
	}

	ListNode* current = this->head->next;
	while (current) {
		if (current->data == data) {
			current->prev->next = current->next;
			current->next->prev = current->prev;
			delete current;
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
	if (!this->head)
		return NULL;

	this->currentIteration = this->head;

	return this->currentIteration->data;
}

void* List::next() {
	this->currentIteration = this->currentIteration->next;
	
	return this->currentIteration ? this->currentIteration->data : NULL;
}

bool List::ended() {
	return this->currentIteration == NULL;
}

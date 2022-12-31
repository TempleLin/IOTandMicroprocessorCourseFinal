#ifndef STACK
#define STACK
struct Node {
  char data;
  Node* next;
};

struct Stack {
  Node* top;
  int length;
};

bool IsEmpty(Stack* stack) { return stack->top == nullptr; }

void Push(Stack* stack, char data) {
  Node* new_node = new Node();
  new_node->data = data;
  new_node->next = stack->top;
  stack->top = new_node;
  stack->length++;
}

void Pop(Stack* stack) {
  if (IsEmpty(stack)) {
    return;
  }
  Node* temp = stack->top;
  stack->top = stack->top->next;
  delete temp;
  stack->length--;
}

char Top(Stack* stack) {
  if (IsEmpty(stack)) {
    return '\0';
  }
  return stack->top->data;
}

void Clear(Stack* stack) {
  while (!IsEmpty(stack)) {
    Pop(stack);
  }
}

int Length(Stack* stack) { return stack->length; }
#endif

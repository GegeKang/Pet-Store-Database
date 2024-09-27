/**~*~*~*~*
   Stack template
 - This stack template is applied for undo-delete for hash table.
 - - Team project group number 3, Unit 4 - Screen output, written by: Katie Leong
*~*/

#ifndef STACK_ADT
#define STACK_ADT

template <class T>
class Stack
{
private:
  // Structure for the stack nodes
  struct StackNode
  {
    T value;         // Value in the node
    StackNode *next; // Pointer to next node
  };

  StackNode *top; // Pointer to the stack top
  int length;

public:
  // Constructor
  Stack()
  {
    top = NULL;
    length = 0;
  }
  // Destructor
  ~Stack();

  // Stack operations:

  // push an item onto stack
  bool push(T);
  // pop an item off the stack
  T pop();
  // peek at the item at the top of the stack
  T peek() { return top->value; }
  //empty the stack
  void empty();
  // return true if the stack is empty
  bool isEmpty()
  {
    return (!getLength());
  }
  // getLength()
  int getLength() { return length; }
};

/**~*~*~*~*
  Member function push inserts the argument onto
  the stack.
  Parameters: T item - value of T to be pushed onto stack
  return: true if the value was able to be put onto the stack
*~**/
template <class T>
bool Stack<T>::push(T item)
{
  StackNode *newNode; // Pointer to a new node

  // Allocate a new node and store value there.
  newNode = new StackNode;
  if (!newNode)
    return false;
  newNode->value = item;

  // Update links and counter
  newNode->next = top;
  top = newNode;
  length++;

  return true;
}

/**~*~*~*~*
  Member function pop deletes the value at the top
  of the stack and returns it.
  Assume stack is not empty.
  Parameters: none
  Return: value that was removed from the top of the stack
*~**/
template <class T>
T Stack<T>::pop()
{
  T topVal = top->value;

  // set a pointer to point to the next location as top will be deleted and inacessible
  StackNode *temp = top->next;
  delete top;
  top = temp;

  // update the counter
  length--;

  return topVal;
}
/**~*~*~*~*
  Empty:
  Traverses the list deleting each node (without calling pop)
  Basically the destructor function but able to be called
  for static objects
*~**/
template <class T>
void Stack<T>::empty()
{
  StackNode* temp;
  while(top)
  {
    temp = top;
    top = top->next;
    delete temp;
    length--;
  }
}

/**~*~*~*~*
  Destructor:
  Traverses the list deleting each node (without calling pop)
*~**/
template <class T>
Stack<T>::~Stack()
{
  StackNode* temp;
  while(top)
  {
    temp = top;
    top = top->next;
    delete temp;
  }
}

#endif

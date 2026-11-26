#include <iostream>

#include <forward_list>

#include <vector>
using namespace std;

struct BufferTag
{
  int one;
  int two;
  int three;
  int four;
};
struct  Page
{
    string x;
};


struct DataEntry
{
  BufferTag bufferTag;
  int BufferId;
};
class BufferTable
{
private:
  int BufferTableSize;
  vector<std::forward_list<DataEntry>> *BuffferTable;

public:
  BufferTable(int BufferTableSize)
  {
    this->BufferTableSize = BufferTableSize;
    this->BuffferTable = new std::forward_list<DataEntry>[BufferTableSize];
  }
  void InsertToBufferTable(DataEntry Element)
  {
    int index=this->HashTag(Element.bufferTag);
    // ✅ Modifies the actual list inside the table
    this->BuffferTable[index].push_front(Element);
  }
  int HashTag(BufferTag bufferTag){
     
     return (bufferTag.four+bufferTag.three+bufferTag.two+bufferTag.one)%7;
  }
  Page GetPage(BufferTag tag){
    int index =this->HashTag(tag);
    int PageIndex=this->BuffferTable->get_allocator(index);
  }
  std::forward_list<DataEntry> *ReturnBufferTableElemnts()
  {
    return this->BuffferTable;
  }
  bool isPageInBufferPool(DataEntry buffer)
  {
  }
  ~BufferTable()
  {
    delete[] this->BuffferTable;
  }
};


// buffer table mocking
/*
buffer table contains a hash table. this hash table is just an array of linked list
each linked list element is an object that has buffer tag and buffer id

*/
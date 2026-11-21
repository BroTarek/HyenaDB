#include <iostream>

#include <forward_list>
using namespace std;


struct DataEntry{
  string bufferTag;
  int BufferId;
};

int main()
{ 
    DataEntry a={"0x123",3};
    DataEntry b={"0x124",4};
    DataEntry c={"0x125",5};
    DataEntry d={"0x126",6};
    DataEntry e={"0x127",7};
    DataEntry f={"0x128",8};
    DataEntry g={"0x129",9};
    DataEntry h={"0x130",10};
    DataEntry i={"0x131",11};
    std::forward_list<DataEntry> list1 = {a,b,c};

    std::forward_list<DataEntry> list2 = {d, e, f};

    std::forward_list<DataEntry> list3 = {g,h,i};
    
    std::forward_list<DataEntry> BuffferTable[4] = {list1, list2, list3};
    for (forward_list<DataEntry> LinkedListElement : BuffferTable)
    {
        for (DataEntry element : LinkedListElement)
        {
            cout << "this buffer tag"<<" "<<element.bufferTag<<" "<<"corresponds to this buffer id in the descriptor"<<" "<<element.BufferId << endl;
        }
    }
    return 0;
}

// buffer table mocking
/*
buffer table contains a hash table. this hash table is just an array of linked list
each linked list element is an object that has buffer tag and buffer id

*/
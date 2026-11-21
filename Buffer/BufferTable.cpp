#include <iostream>

#include <forward_list>
using namespace std;


struct DataEntry{
  string bufferTag;
  int BufferId;
};
class BufferTable{
    private:
      int BufferTableSize;
      std::forward_list<DataEntry>* BuffferTable;
    public:
      BufferTable(int BufferTableSize){
          this->BufferTableSize=BufferTableSize;
          this->BuffferTable=new std::forward_list<DataEntry>[BufferTableSize];
      }
      void InsertToBufferTable(DataEntry Element,int index){
       // ✅ Modifies the actual list inside the table
this->BuffferTable[index].push_front(Element);

      }
      std::forward_list<DataEntry>* ReturnBufferTableElemnts(){
        return this->BuffferTable;
      }
      ~BufferTable(){
        delete[] this->BuffferTable;
      }
    
      
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
    DataEntry List[9] = {a,b,c,d,e,f,g,h,i};
   
    BufferTable BuffTable(9);
    int index=0;
    for(int i=0;i<9;i++){
       if(i%2==0){
        index=2;
        BuffTable.InsertToBufferTable(List[i],index);
       }
       if(i%2==1){
        index=3;
        BuffTable.InsertToBufferTable(List[i],index);
       }
       
    };
    auto* LinkedListElements = BuffTable.ReturnBufferTableElemnts();
    for (int i=0;i<9;i++){
        for(auto element: LinkedListElements[i]){
            std::cout<<"this tag"<<" "<<element.bufferTag<<" "<<"corresponds to this id"<<""<<element.BufferId<<endl;
        }
    }
    
    return 0;
}

// buffer table mocking
/*
buffer table contains a hash table. this hash table is just an array of linked list
each linked list element is an object that has buffer tag and buffer id

*/
#include <iostream>
#include <cstdint>
#include <vector>
#include <forward_list>

using namespace std;
struct DescriptorElement{
  int   pinCount=0;
  bool  dirtyBit=0;
  int   NumberOfWrites=0;
  int	buf_id;			/* buffer's index number (from 0) */
  int	freeNext;
  string buffer_tage;	
}; 
struct  Page
{
    string x;
};

class DescriptorTable
{
private:
    int ArraySize;
    int freeList=-1;
    std::vector<DescriptorElement> DescriptorTableArray;
public:
    DescriptorTable(int ArraySize){
        this->ArraySize=ArraySize;
        this->DescriptorTableArray.resize(this->ArraySize);
    };
    void insertNewBufferFrame(DescriptorElement element){
        if(this->DescriptorTableArray.size()>=this->ArraySize){
            DescriptorElement victim=this->SelectVictim(this->DescriptorTableArray);
            this->deleteBufferFrame(victim);
        }
        this->DescriptorTableArray.push_back(element);
        this->freeList++;
        this->DescriptorTableArray[this->freeList-1].freeNext=this->freeList-1;
        this->DescriptorTableArray[this->freeList].freeNext=-1;
    }
    void deleteBufferFrame(DescriptorElement victim){
        this->DescriptorTableArray.erase(this->DescriptorTableArray.begin()+victim.buf_id);
    }
    DescriptorElement SelectVictim(std::vector<DescriptorElement> DescTable){
        bool isVictimSelected=false;
        while(!isVictimSelected){

            for(DescriptorElement desc: DescTable){
                if(desc.pinCount==0){
                    if(desc.NumberOfWrites==0){
                        isVictimSelected=true;
                        return desc;
                    }
                    desc.NumberOfWrites--;
                }
            }
        }
        }
};
class BufferPool{
    private:
     int bufferPoolSize;
     Page page;
     std::vector<Page> bufferPool;
    public:
     BufferPool(int bufferPoolSize){
        this->bufferPoolSize=bufferPoolSize;
        this->bufferPool.resize(this->bufferPoolSize);
     }
     Page getPage(int index){
        return bufferPool[index];
     }
     void insertInBufferPool(Page page){
        this->bufferPool.push_back(page);
     }
     void deleteFromBufferPool(int bufferFrame){
        this->bufferPool.erase(this->bufferPool.begin()+bufferFrame);
     }
     Page getPage(){}
};
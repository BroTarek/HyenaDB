// #include <iostream>
// #include <cstdint>
// #include <vector>
// #include <forward_list>

// struct BufferDesc
// {
	
    
// 	int		buf_id;			/* buffer's index number (from 0) */
//     int pinCount;//number of Threads reading for this buufer frame at thime moment
//     int dirtyBit;
//     int numberOfWrites; 
//     int		freeNext;		/* link in freelist chain */
// };
// class DescriptorTable{
//     private:
//        int DescriptorTableSize;
//        std::vector<BufferDesc> DescriptorTableArray;
//     //    std::freelist<
//        int nextVictim;
//     public:
//         DescriptorTable(int DescriptorTableSize){
//         this->DescriptorTableSize=DescriptorTableSize;
//         this->DescriptorTableArray.reserve(this->DescriptorTableSize);
//        }
//        std::vector<BufferDesc> getDescriptorTableArray(){return this->DescriptorTableArray;}
//        void InsertDescElement(BufferDesc BufferDescElement){
//          this->DescriptorTableArray.push_back(BufferDescElement);
//        }
// };

// /*
// if pinCount is zero 
//   if numberOfWrites is zero , retrun the PageDescriptor
//     else descrements the numberOf writes
// */


// int main(){
//     DescriptorTable dt(6);
//     SelectVictim(dt.getDescriptorTableArray());
//     return 0;
// }
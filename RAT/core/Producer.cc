#include <RAT/core/Producer.hh>
#include <RAT/core/ProcBlock.hh>

namespace RAT {


Producer::Producer()
{
  mainBlock = 0;
}

Producer::Producer(ProcBlock *block)
{
  SetMainBlock(block);
}

ProcBlock *Producer::GetMainBlock()
{
  return mainBlock;
}

void Producer::SetMainBlock(ProcBlock *block)
{
  mainBlock = block;
}


} // namespace RAT

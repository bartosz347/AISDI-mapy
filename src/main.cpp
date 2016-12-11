#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

const int MAX_KEY_VALUE = 100000;

const int ITERATING = 4;
const int ACCESSING = 8;

namespace
{

template <typename K, typename V>
using Map = aisdi::HashMap<K, V>;

void perfomTest(int noElements, int whichOperations)
{
  (void)whichOperations;
  std::vector < std::pair <int, long int> > tab(noElements);

  // Array with noElements, element is a pair key-value
  for(int i = 0; i < noElements; i++)
    tab[i] = std::make_pair((rand() % MAX_KEY_VALUE), (rand() % 100000));

  Map<int, long int> map;


  // 1) adding elements to the Map
  for(auto it = tab.begin(); it < tab.end(); it++)
    map[it->first] = it->second;


  // 2) iterating through the Map
  if(whichOperations & ITERATING) {
    std::cout << "iterating" << std::endl;
    for(auto it = map.begin(); it != map.end(); it++)
      (void)it->first;
  }

  // 3) accessing random keys
  if(whichOperations & ACCESSING) {
    std::cout << "accessing" << std::endl;
    for(int i = 0; i < noElements; i++)
      (void)map[ tab[rand() % noElements].first ];
  }
}

} // namespace

int main(int argc, char** argv)
{
  srand(time(0));
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;

  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest(1000, ACCESSING|ITERATING);
  return 0;
}

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

template <typename M>
void perfomTest(int noElements, int whichOperations, M map) // we want fresh one in each iteration
{
  std::vector < std::pair <int, long int> > tab(noElements);

  // Array with noElements, element is a pair key-value
  for(int i = 0; i < noElements; i++)
    tab[i] = std::make_pair((rand() % MAX_KEY_VALUE), (rand() % 100000));

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
  // usage ./aisdiMaps repeat_count T|H
  srand(time(0));
  if(argc < 3) return -1;
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;
  const int operation = ACCESSING|ITERATING;
  const int noElements = 1000;



  if((*argv[2]) == 'T') {
    std::cout << "TreeMap" << std::endl;
    aisdi::TreeMap<int, long int> map;
    for (std::size_t i = 0; i < repeatCount; ++i)
      perfomTest< aisdi::TreeMap<int, long int> > (noElements, operation, map);
  }
  else if((*argv[2]) == 'H') {
    std::cout << "HashMap" << std::endl;
    aisdi::HashMap<int, long int> map;
    for (std::size_t i = 0; i < repeatCount; ++i) {
      perfomTest< aisdi::HashMap<int, long int> > (noElements, operation, map);
    }
  }
  return 0;
}

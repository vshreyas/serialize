#include "StreamWriter.hpp"

/** @brief serializes any STL vector
  * by writing the size first
  * and then the vector elements
*/
template <typename T>
 void serialize(StreamWriter& w, const std::vector<T> & vec_data) {
      size_t vec_size = vec_data.size();
      //w.stream->write(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
      w>>vec.size();
      for(auto it = vec_data.begin();it !=vec_data.end();++it) {
        w<<*it;
      }
  }

/**
 *
 *
*/
template<typename T1, typename T2>
void serialize(StreamWriter& w, const std::pair<T1, T2>& pair_data) {
      w<<pair_data.first<<pair_data.second;
  }

/**
 *
 *
*/
template<typename T1, typename T2>
void serialize(BinaryStreamWriter& w, const std::map<T1, T2>& map_data) {
    size_t map_size = map_data.size();
    w<<map_size;
    for(auto it = map_data.begin();it !=map_data.end();++it) {
        w<<(*it).first<<(*it).second;
    }
  }

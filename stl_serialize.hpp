/**
 * @file    stl_serialize.hpp
 * @author Shreyas Vinayakumar
 * @brief Serializes any C++ object
 */
#ifndef STL_SERIALIZE_HPP
#define STL_SERIALIZE_HPP
#include "StreamWriter.hpp"
#include <cstddef>
#include <type_traits>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

/** @brief serializes any STL vector
  * by writing the size first
  * and then the vector elements
  * @param reference to a Writer w derived from a StreamWriter and a vector
*/
template <typename Writer, typename T>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
 serialize(Writer& w, const std::vector<T> & vec_data) {
      size_t vec_size = vec_data.size();
      //w.stream->write(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
      w<<vec_size;
      for(auto it = vec_data.begin();it !=vec_data.end();++it) {
        w<<*it;
      }
  }

/**
 * Serialization of std::pair
 * The first followd by second
*/
template<typename Writer, typename T1, typename T2>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
serialize(Writer& w, const std::pair<T1, T2>& pair_data) {
      w<<pair_data.first<<pair_data.second;
  }

/**
 * Serialize std::map
 * write each pair
*/
template<typename Writer, typename T1, typename T2>
typename std::enable_if<std::is_base_of<StreamWriter, Writer>::value>::type
serialize(Writer& w, const std::map<T1, T2>& map_data) {
    size_t map_size = map_data.size();
    w<<map_size;
    //cout<<map_size<<endl;
    for(auto it = map_data.begin();it !=map_data.end();++it) {
        w<<(*it).first<<(*it).second;
        //cout<<(*it).first<<(*it).second;
    }
  }


template <typename Reader, typename T>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader& r, std::vector<T>& vec_data) {
      size_t vec_size_read;
      //r.stream->read(reinterpret_cast<char*>(&vec_size_read), sizeof(vec_size_read));
      r>>vec_size_read;
      // check vec_data size here and throw an exception if it is not 0(ie not empty)
      int i;
      T t;
      for(i = 0; i<vec_size_read;i++){
        //r.stream->read(reinterpret_cast<char*>(&t), sizeof(t));
        r>>t;
        vec_data.push_back(t);
      }
}

template <typename Reader, typename T1, typename T2>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader& r, std::pair<T1, T2>& pair_data) {
    r>>pair_data.first>>pair_data.second;
}

template<typename Reader, typename T1, typename T2>
typename std::enable_if<std::is_base_of<StreamReader, Reader>::value>::type
deserialize(Reader& r, std::map<T1, T2>& map_data) {
    size_t map_size_read;
    r>>map_size_read;
    //check size
    std::pair<T1, T2> p;
    std::pair<typename std::map<T1, T2>::iterator,bool> ret;
    int i;
    for(i = 0; i<map_size_read;i++){
        r>>p;
        ret = map_data.insert(p);
        if(ret.second == false) {
            //throw exception here
        }
    }
}
#endif

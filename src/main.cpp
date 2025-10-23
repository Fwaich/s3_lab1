#include "Sequnces.hpp"
#include "UniquePtr.hpp"
#include "ReadOnlyStream.hpp"
#include <iostream>
#include <memory>


int main() {
    try {

        UniquePtr<Sequence<int>> vec(new VectorSequence<int>());
        // VectorSequence<int> vec;
        // std::unique_ptr<Sequence<int>> vec = std::make_unique<VectorSequence<int>>(VectorSequence<int>());

        for (int i = 0; i < 120; i++) {
            vec->append(i);
        } 
        std::cout << vec->to_string() << std::endl;

        // ArraySequence<int> a;
        
        // for (int i = 0; i < 120; i++) {
        //     a.append(i);
        // }
        // ArraySequence<int> b(a);

        // for (int i = 0; i < 10; i++) {
        //     std::cout << b.get(i) << std::endl;
        // }

        // std::cout << b.to_string() << std::endl; 
        
        ReadOnlyStream<int> stream(vec.release());
        while (!stream.is_end_off_stream()) {
            std::cout << stream.read() << std::endl;  
        }
        std::cout << "End of stream" << std::endl;

        // std::cout << a.to_string() << std::endl; 
        // std::cout << a.get_size() << std::endl; 
    
        int arr_int[] = {1, 2, 3, 4, 5};
        VectorSequence<int> a(arr_int, 5);

        for (int i = 0; i < 100; ++i)
            a.append(i);

        std::cout << a.to_string() << std::endl;

    } catch(const out_of_range& e){
        std::cout << e.what() << std::endl; 
    }  catch(const data_is_null& e){
        std::cout << e.what() << std::endl; 
    }

    return 0;
}
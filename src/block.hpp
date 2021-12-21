#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <functional>


namespace blockchain {
    
    namespace details {
        
        template <class T>
        void hash(size_t& seed, const T& object) {
            seed ^= std::hash<T>{}(object) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        void hash_combine(std::size_t& seed, const time_t& timestamp, 
            const std::string& data, const uint64_t& previous_hash, 
            const uint64_t& nounce) {
            hash(seed, timestamp);
            hash(seed, data);
            hash(seed, previous_hash);
            hash(seed, nounce);
        }
    }

    class Block {
    public:

        Block() : timestamp_(std::time(nullptr)), prev_hash_(0) { }
        
        Block(std::string data) : timestamp_(std::time(nullptr)), 
            data_(std::move(data)), prev_hash_(0) { }
        
        Block(std::string data, uint64_t previous_hash) : 
            timestamp_(std::time(nullptr)), data_(std::move(data)), 
            prev_hash_(previous_hash) { } 

        Block(std::string data, uint64_t previous_hash, uint64_t nounce) : 
            timestamp_(std::time(nullptr)), data_(std::move(data)), 
            prev_hash_(previous_hash), nounce_(nounce) { }
            
        Block(std::string data, uint64_t previous_hash, uint64_t nounce, 
              std::time_t timestamp) : 
            timestamp_(timestamp), data_(std::move(data)), 
            prev_hash_(previous_hash), nounce_(nounce) { } 

        Block(const Block& block) = default;
        Block(Block&&) = default;
        
        Block& operator=(const Block& block) = default;
        Block& operator=(Block&&) = default;

        std::time_t get_timestamp() const {
            return timestamp_;
        }

        std::string get_data() const {
            return data_;
        }

        uint64_t get_hash() const {
            uint64_t hash = 0;
            details::hash_combine(hash, timestamp_, data_, prev_hash_, nounce_);
            return hash;
        }

        uint64_t get_previous_hash() const {
            return prev_hash_;
        }

        uint64_t get_nounce() const {
            return nounce_;
        }

        bool operator==(const Block& block) const {
            return timestamp_ == block.timestamp_ && data_ == block.data_ && 
                   prev_hash_ == block.prev_hash_ && nounce_ == block.nounce_;
        }

        bool operator!=(const Block& block) const {
            return !(*this == block);
        }
        
        friend inline std::ostream& operator<<(std::ostream& os, 
                                               const Block& block) {
            os << "timestamp " << std::asctime(std::localtime(&block.timestamp_));
            os << "data " << block.data_ << std::endl;
            os << "prev hash " << block.prev_hash_ << std::endl;
            os << "hash " << block.get_hash() << std::endl;
            os << "nounce " << block.nounce_ << std::endl;
            return os; 
        }

        ~Block() = default;

    private:
        time_t timestamp_{};
        std::string data_{};
        uint64_t prev_hash_{};
        uint64_t nounce_{};  
    };
}
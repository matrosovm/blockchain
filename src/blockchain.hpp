#pragma once

#include "block.hpp"

#include <vector>

namespace blockchain {

    class BlockChain {
    public:
        BlockChain() {
            chain_.push_back(Block{});
        };
        
        BlockChain(const BlockChain&) = default;
        BlockChain(BlockChain&&) = default;

        BlockChain& operator=(const BlockChain&) = default;
        BlockChain& operator=(BlockChain&&) = default;

        void push_back(const Block& block) {
            chain_.push_back(block);
        }

        size_t size() const {
            return chain_.size();
        }

        const Block& get_block(size_t index) const {
            return size() > index ? chain_[index] : chain_[0];
        }
        
        Block& get_block(size_t index) {
            return size() > index ? chain_[index] : chain_[0];
        }

        Block& operator[](size_t index) {
            return get_block(index);
        }

        void pop() {
            if (chain_.size() > 1) {
                chain_.pop_back();
            }
        }

        Block& back() {
            return chain_.back();
        }
        
        const Block& back() const {
            return chain_.back();
        }

        bool block_validation(size_t index) const {
            if (index == 0 || size() > index) {
                return false;
            }
            return chain_[index - 1].get_hash() == chain_[index].get_hash();
        }

        Block check_blockchain_validation() const {
            for (size_t i = 1; i < size(); ++i) {
                if (chain_[i - 1].get_hash() != chain_[i].get_previous_hash()) {
                    return chain_[i];
                }
            }
            return chain_[0];
        }

        BlockChain get_part(size_t index, size_t count) const {
            BlockChain part;
            uint64_t prev_hash = part[0].get_hash();
            for (size_t i = index > 0 ? index : 1; 
                 i < count + index && i < chain_.size(); ++i) {
                const Block& tmp = chain_[i];
                part.push_back(Block(tmp.get_data(), 
                    prev_hash, tmp.get_nounce(), tmp.get_timestamp()));
                prev_hash = part[i].get_hash();
            }
            return part;
        }

        BlockChain& concatenate(const BlockChain& blockchain) {
            for (size_t i = 1; i < blockchain.size(); ++i) {
                push_back(blockchain.get_block(i));
            }
            return *this;
        }

        friend inline std::ostream& operator<<(std::ostream& os, 
                                               const BlockChain& blockchain) {
            for (size_t i = 0; i < blockchain.size(); ++i) {
                os << "block " << i << ":" << std::endl;
                os << blockchain.get_block(i);
                os << std::endl;
            }
            return os;
        }

    private:
        std::vector<Block> chain_;
    };

}
#include "../src/block.hpp"
#include "../src/blockchain.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <cassert>

using namespace blockchain;

void sleep(uint32_t seconds) {
    std::chrono::duration<int, std::milli> timespan(seconds * 1000);
    std::this_thread::sleep_for(timespan);
}

void compare_output(std::stringstream& os, 
    std::vector<std::string>& expected) {
    int i = 0;
    int j = 0;
    std::string tmp;
    while (std::getline(os, tmp) && j < 3) {
        if (i != 0 && i != 1 && i != 4) {
            assert(tmp == expected[j]);
            ++j;
        }
        ++i;
    }
}

void test_block() {
    Block bl;
    sleep(1);
    Block bl1("block1", bl.get_hash());
    sleep(1);
    Block bl2("block2", bl1.get_hash(), 1);
    
    std::vector<std::string> res;
    std::stringstream os;
    {
        os << "block" << std::endl;
        os << bl;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
    }

    {
        os << "block1" << std::endl;
        os << bl1 << std::endl;
        res = {"data block1", "prev hash " + std::to_string(bl.get_hash()), 
            "nounce 0"};
        compare_output(os, res);
    }

    {
        os << "block2" << std::endl;
        os << bl2 << std::endl;
        res = {"data block2", "prev hash " + std::to_string(bl1.get_hash()), 
            "nounce 1"};
        compare_output(os, res);
    }
    std::cout << "TEST BLOCK OK" << std::endl;
}

void test_blockchain() {
    BlockChain chain;
    sleep(1);
    Block bl1("block1", chain.back().get_hash());
    sleep(1);
    Block bl2("block2", bl1.get_hash(), 1);
    sleep(1);
    Block bl3("block3", bl2.get_hash(), 1);
    
    std::vector<Block> blocks{bl1, bl2, bl3};
    for (auto& block : blocks) {
        chain.push_back(block);
    }

    std::vector<std::string> res;
    {
        std::stringstream os;
        os << chain;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
        res = {"data block1", "prev hash " + std::to_string(chain[0].get_hash()), 
            "nounce 0"};
        compare_output(os, res);
        res = {"data block2", "prev hash " + std::to_string(chain[1].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block3", "prev hash " + std::to_string(chain[2].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
    }
    {
        assert(chain.check_blockchain_validation() == chain[0]);
    }

    BlockChain chain2;
    Block bl4("block4", bl3.get_hash(), 1);
    Block bl5("block5", bl4.get_hash(), 1);
    
    blocks = {bl4, bl5};
    for (auto& block : blocks) {
        chain2.push_back(block);
    }

    {
        assert(chain2.check_blockchain_validation() != chain[0]);
    }
    {
        chain.concatenate(chain2);
        std::stringstream os;

        os << chain;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
        res = {"data block1", "prev hash " + std::to_string(chain[0].get_hash()), 
            "nounce 0"};
        compare_output(os, res);
        res = {"data block2", "prev hash " + std::to_string(chain[1].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block3", "prev hash " + std::to_string(chain[2].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block4", "prev hash " + std::to_string(chain[3].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block5", "prev hash " + std::to_string(chain[4].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
    }
    {
        assert(chain.check_blockchain_validation() == chain[0]);
    }
    {
        chain.concatenate(chain2);
        std::stringstream os;

        os << chain;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
        res = {"data block1", "prev hash " + std::to_string(chain[0].get_hash()), 
            "nounce 0"};
        compare_output(os, res);
        res = {"data block2", "prev hash " + std::to_string(chain[1].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block3", "prev hash " + std::to_string(chain[2].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block4", "prev hash " + std::to_string(chain[3].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block5", "prev hash " + std::to_string(chain[4].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block4", "prev hash " + std::to_string(chain[5].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block5", "prev hash " + std::to_string(chain[6].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
    }
    {
        assert(chain.check_blockchain_validation() == chain[0]);
    }

    {
        chain = chain.get_part(1, 5);
        std::stringstream os;

        os << chain;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
        res = {"data block1", "prev hash " + std::to_string(chain[0].get_hash()), 
            "nounce 0"};
        compare_output(os, res);
        res = {"data block2", "prev hash " + std::to_string(chain[1].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block3", "prev hash " + std::to_string(chain[2].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block4", "prev hash " + std::to_string(chain[3].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block5", "prev hash " + std::to_string(chain[4].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
    }
    {
        assert(chain.check_blockchain_validation() == chain[0]);
    }

    {
        BlockChain chain3(bl1, bl2, bl3);
        std::stringstream os;
        os << chain3;
        res = {"data ", "prev hash 0", "nounce 0"};
        compare_output(os, res);
        res = {"data block1", "prev hash " + std::to_string(chain3[0].get_hash()), 
            "nounce 0"};
        compare_output(os, res);
        res = {"data block2", "prev hash " + std::to_string(chain3[1].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        res = {"data block3", "prev hash " + std::to_string(chain3[2].get_hash()), 
            "nounce 1"};
        compare_output(os, res);
        assert(chain3.check_blockchain_validation() == chain3[0]);
    }

    std::cout << "TEST BLOCKCHAIN OK" << std::endl;
}

int main() {
    test_block();
    test_blockchain();
    return 0;
}
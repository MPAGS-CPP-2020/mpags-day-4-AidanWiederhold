#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */

using CoordPair = std::pair<size_t, size_t>; //To store the 2D coords for each letter
using Char2CoordMap = std::map<char, CoordPair>; //To create a letter to coord map
using Coord2CharMap = std::map<CoordPair, char>; //To create a coord to letter map

class PlayfairCipher{
    public:
        /**
        * Create a new PlayfairCipher with the given key
        *
        * \param key the key to use in the cipher
        */
        PlayfairCipher(const std::string& key);
        /**
        * Use the key input to set up the PlayfairCipher grid
        *
        * \param key the key to use in the cipher
        */
        void setKey(const std::string& key);
        /**
        * Apply the cipher to the provided text
        *
        * \param inputText the text to encrypt or decrypt
        * \param cipherMode whether to encrypt or decrypt the input text
        * \return the result of applying the cipher to the input text
        */
        std::string applyCipher(const std::string& inputText, const CipherMode cipherMode) const;

    private:
    /// The cipher key
    std::string key_ = "";
    /// The Alphabet
    const std::vector<char> alphabet_ = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    /// Char -> coordinate map
    Char2CoordMap char2CoordMap_;
    ///Coordinate -> char map
    Coord2CharMap coord2CharMap_;
};

#endif
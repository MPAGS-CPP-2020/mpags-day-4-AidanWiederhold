// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key)
  : key_{key}
{
    setKey(key_);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;
    
    // Append the alphabet to the key
    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    // Remove non-alpha characters
    auto notAlpha = [] (char inChar) {return !std::isalpha(inChar);};
    auto alphaIter = std::remove_if(key_.begin(),key_.end(),notAlpha);
    key_.erase(alphaIter,key_.end());

    // Change J -> I
    auto ijSwap = [] (char inChar)
    {
        if(inChar=='J'){return 'I';}
        else{return inChar;}
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), ijSwap);

    // Remove duplicated letters
    std::string encountered{""};
    //This returns whether or not a given character at a particular position should be removed from the key
    auto removeDuplicate = [&] (char inChar)
    {
        if(encountered.find(inChar)!=std::string::npos)
        {
            //the char has been seen before so must be removed
            return true;
        }
        else
        {
            //char has not been seen before so is added to the list of seen but told not to be removed
            encountered+=inChar;
            return false;
        }
    };
    auto duplicateIter = std::remove_if(key_.begin(),key_.end(),removeDuplicate);
    key_.erase(duplicateIter,key_.end());
    
    // Store the coords of each letter
    // Store the playfair cipher key map
    // I will do both steps in one go since I think this makes sense to do in the same loop
    //using CoordPair = std::pair<size_t, size_t>; //To store the 2D coords for each letter

    // Initialize variables to calculate and store coords
    // I will be counting starting from zero with columns counted left
    // to right and rows top to bottom to follow the order of the string
    size_t rowN{};
    size_t colN{};
    CoordPair coords{};
    for(size_t i{0}; i < 25; ++i)
    {
        // Determine the coordinates
        colN = (i%5);
        rowN = (i-colN)/5;
        coords = {rowN,colN};

        //Store the char -> coord map
        char2CoordMap_[key_[i]] = coords;
        
        // Store the coord -> char map
        coord2CharMap_[coords] = key_[i];
    }

    /*
    //Remember to delete this
    std::cout<<key_<<std::endl;
    for(auto p : char2CoordMap_)
    {
        std::cout << p.first << ":" << p.second.first << "," << p.second.second << std::endl;
    }
    for(auto p: coord2CharMap_)
    {
        std::cout << p.second << ":" << p.first.first << "," << p.first.second << std::endl;
    }
    */
}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) const
{
    // Prepare the intermediate and output strings
    std::string intermediateText{inputText};
    std::string outputText{""};
    // Change J -> I
    auto ijSwap = [] (char inChar)
    {
        if(inChar=='J'){return 'I';}
        else{return inChar;}
    };
    std::transform(intermediateText.begin(), intermediateText.end(), intermediateText.begin(), ijSwap);
    // If repeated chars in a digraph add an X or Q if XX
    for(size_t i{1}; i<intermediateText.size(); ++i)
    {
        if(intermediateText[i] == intermediateText[i-1])
        {
            if(intermediateText[i]=='X')
            {
                intermediateText.insert(i, 1, 'Q');
            }
            else
            {
                intermediateText.insert(i, 1, 'X');
            }
        }
    }
    // if the size of input is odd, add a trailing Z
    if(intermediateText.size()%2==1)
    {
        intermediateText.insert(intermediateText.size(), 1, 'Z');
    }
    // Loop over the input in Digraphs
    // - Find the coords in the grid for each digraph
    //auto coords0iter{char2CoordMap_.find("A")}; // Not sure how to initialise with auto without specifying a value to initialise to so will use A as a placeholder
    //auto coords1iter{char2CoordMap_.find("A")};
    CoordPair coords0;
    CoordPair coords1;
    size_t coords0ColumnHolder; //To hold the column value for the coords0 if the coords form a rectangle
    size_t increment{1};
    char char0;
    char char1;
    for(size_t i{0}; i<intermediateText.size(); i+=2)
    {
        coords0 = (*char2CoordMap_.find(intermediateText[i])).second;
        coords1 = (*char2CoordMap_.find(intermediateText[i+1])).second;
        //coords0.second = (*char2CoordMap_.find(intermediateText[i])).second;
        //coords1.second = (*char2CoordMap_.find(intermediateText[i+1])).second;
        // - Apply the rules to these coords to get 'new' coords
        // Define a lambda for incrementation/reduction
        auto incrementWrap = [] (size_t coord, CipherMode mode, size_t one)
        {
            if(mode==CipherMode::Encrypt)
            {
                if(coord==4)
                {
                    return coord*0;
                }
                else
                {
                    return coord+one;
                }
            }
            else
            {
                if(coord==0)
                {
                    return one*4;
                }
                else
                {
                    return coord-one;
                }

            }
        };
        // If the letters are adjacent on a row
        if(coords0.first==coords1.first && abs(coords0.second-coords1.second))
        {
            coords0.first = incrementWrap(coords0.first, cipherMode, increment);
            coords1.first = incrementWrap(coords1.first, cipherMode, increment);
        }
        // If the letters are adjacent on a column
        else if(coords0.second==coords1.second && abs(coords0.first-coords1.first))
        {
            coords0.second = incrementWrap(coords0.second, cipherMode, increment);
            coords1.second = incrementWrap(coords1.second, cipherMode, increment);
        }
        // If the above aren't true then the coords must form a rectangle
        else
        {
            coords0ColumnHolder = coords0.second;
            coords0.second = coords1.second;
            coords1.second = coords0ColumnHolder;
        }
        // - Find the letter associated with the new coords
        char0 = (*coord2CharMap_.find(coords0)).second;
        char1 = (*coord2CharMap_.find(coords1)).second;
        outputText += char0;
        outputText += char1;
    }
    // return the text
    return outputText;
}
#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  class Trie
  {
  public:
    Trie()
    {
      root.endOfWord = false;
      root.subWord   = "";
    }

    void insert(const bs::String& word)
    {
      BS_LOG(Info, Uncategorized, "[Console/Trie] Inserting {0}!", word);
      Node& curr = root;

      for (int i = 0; i < word.size(); i++)
      {
        char c = word.at(i);
        if (curr.children.find(c) == curr.children.end())
        {
          BS_LOG(Info, Uncategorized, "[Console/Trie] Inserting Node for char {0}!", c);
          Node newNode;
          newNode.endOfWord = false;
          newNode.subWord   = curr.subWord + c;
          curr.children[c]  = newNode;
        }
        curr = curr.children[c];  // am I copying it here?, but it is a reference right now
      }
      curr.endOfWord = true;
    }

    bs::Vector<bs::String> findSuggestions(const bs::String& word)
    {
      bs::Vector<bs::String> ret;
      Node curr = root;

      for (int i = 0; i < word.size(); i++)
      {
        char c = word.at(i);

        if (curr.children.find(c) == curr.children.end())
        {
          // TODO: Does not find the node?
          BS_LOG(Info, Uncategorized, "[Console/Trie] Did not find Node with char {0}!", c);
          // Leave early as there are not reference for this word
          return ret;
        }

        curr = curr.children[c];
      }

      // Check every node (curr and below) to see if subWord==true)
      ret = traverse(curr);

      ret.push_back("lol");
      return ret;
    }

  private:
    struct Node
    {
      bs::Map<char, Node> children;
      bool endOfWord;
      bs::String subWord;
    };

    // recursive function
    bs::Vector<bs::String> traverse(Node node)
    {
      bs::Vector<bs::String> ret;
      BS_LOG(Info, Uncategorized, "[Console/Trie] Traversing to Node with subword {0}!",
             node.subWord);
      if (node.endOfWord)
      {
        ret.push_back(node.subWord);
      }

      for (auto it = node.children.begin(); it != node.children.end(); it++)
      {
        bs::Vector<bs::String> tmp = traverse(it->second);
        ret.insert(ret.end(), tmp.begin(), tmp.end());
      }

      return ret;
    }

    Node root;
  };
}  // namespace REGoth
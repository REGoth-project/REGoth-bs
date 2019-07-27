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
      Node* curr = &root;

      for (int i = 0; i < word.size(); i++)
      {
        char c = word.at(i);
        if (curr->children.find(c) == curr->children.end())
        {
          Node newNode;
          newNode.endOfWord = false;
          newNode.subWord   = curr->subWord + c;
          curr->children[c] = newNode;
        }
        curr = &curr->children[c];
      }
      curr->endOfWord = true;
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
          // Leave early as there are not reference for this word
          return ret;
        }

        curr = curr.children[c];
      }

      // Check every node (curr and below) to see if subWord==true)
      ret = traverse(curr);

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
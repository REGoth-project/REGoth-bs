#pragma once
#include <BsPrerequisites.h>

namespace REGoth
{
  /**
   * This class is an efficient search-tree for strings.
   * You can insert words that are transformed into character streams and then broken up into nodes
   * identified by the respective character.
   *
   * Currently used for generating auto-complete suggestions for the Console component.
   */
  class Trie
  {
  public:
    Trie()
    {
      root.endOfWord = false;
      root.subWord   = "";
    }

    /**
     * Insert the given word into the Trie by splitting it into its characters and inserting nodes
     * for each one.
     *
     * @param  word
     *            Word that shall be inserted.
     */
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
          newNode.subWord = curr->subWord + c;  // TODO: We realistically only need to save the whole
                                                // word were endOfWord will be set to true?
          curr->children[c] = newNode;
        }
        curr = &curr->children[c];
      }
      curr->endOfWord = true;
    }

    /**
     * @return A vector of words that are auto-complete suggestions for the given word(-prefix).
     *
     * @param  word
     *            String to which to find all auto-complete suggestions in the Trie.
     */
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

    /**
     * @return A vector of words that are auto-complete suggestions for the given node.
     *         Traversing all children of the given node, checking wether isEndOfWord==true and
     *         adding the full word saved at that node to the return list.
     *
     * @param  node
     *            Start node to traverse downwards from.
     */
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
#pragma once
#include <vector>
#include "Move.h"

namespace BookMoves {
	std::vector<Node*> roots;
	struct Node {
		Move move;
		std::vector<Node*> children;
	};
	static Node* createRoot(Move move) {
		Node* root = new Node;
		root->move = move;
		roots.push_back(root);
		return root;
	}
	static Node* addChild(Node* node, Move move) {
		Node* temp = new Node;
		temp->move = move;
		(node->children).push_back(temp);
		return temp;
	}
	std::vector<Node*> getRoots() {
		return roots;
	}
	static Node* findChild(Node* node, Move move) {
		for (int i = 0; i < node->children.size; i++) {
			if ((node->child[i]->move)
		}
		return NULL;
	}
}
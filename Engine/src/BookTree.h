#pragma once
#include <vector>
#include "Move.h"

namespace BookMoves {

	struct Node {
		static inline std::vector<Node*> roots;
		Move move;
		std::vector<Node*> children;
		
		static Node* createRoot(Move move) {
			Node* root = new Node;
			root->move = move;
			roots.push_back(root);
			return root;
		}
		static std::vector<Node*> getRoots() {
			return roots;
		}
		Node* addChild(Move move) {
			Node* temp = new Node;
			temp->move = move;
			(this->children).push_back(temp);
			return temp;
		}
		void addChildren(std::vector<Move> moves) {
			for (int i = 0; i < moves.size(); i++) {
				Node* temp = new Node;
				temp->move = moves[i];
				(this->children).push_back(temp);
			}
		}
		Node* findChild(Move move) {
			for (int i = 0; i < this->children.size(); i++) {
				if ((this->children[i]->move) == move) {
					return this->children[i];
				}
			}
			return NULL;
		}
	};
	static void initTree() {
		Node* root1 = Node::createRoot({ { 4, 1 }, { 4, 3 } }); //e4
		root1->addChild({ { 2, 5 }, { 2, 4 } });//caro-kann
		root1->children[0]->addChildren({ { { 1, 0 }, { 2, 2 } }, { { 3, 1 }, { 3, 3 } } });
		root1->children[0]->children[0]->addChild({ {3,6},{3,4} });
		root1->children[0]->children[1]->addChild({ {3,6},{3,4} });

		root1->addChild({{4,6},{4,4}});//vienna
		root1->children[1]->addChildren({ {{ 3, 1 }, { 3, 2 } },{{1,0},{2,2}} });

		Node* root2 = Node::createRoot({ { 3, 1 }, { 3, 3 } }); //d4
		root2->addChild({ {3,6},{3,4} }); //london
		root2->children[0]->addChildren({ {{2,0},{5,3}} });
		root2->children[0]->children[0]->addChildren({ { {2,7},{5,4} },{{6,7},{5,5}} });
		root2->children[0]->children[0]->children[0]->addChild({ {6,0},{5,2} });
		root2->children[0]->children[0]->children[1]->addChild({ {6,0},{5,2} });

		root2->addChild({{6,6},{6,5}}); //king's indian
		root2->children[1]->addChildren({ {{2,1},{2,3}},{{6,0},{5,2}} });
		root2->children[1]->children[0]->addChildren({ { {5,7},{6,6} } });
		root2->children[1]->children[1]->addChildren({ { {5,7},{6,6} } });
	}

}
#pragma once

#include <vector>

#include "Move.h"


namespace BookMoves {


	struct Node {
	public:
		
		
		Node* addChild(Move move, std::string name = "") {
			Node* child = new Node();
			child->move = move;
			child->openingName = name != "" ? name : openingName;
			children.push_back(child);
			return child;
		}


		void addChildren(const std::vector<Move>& moves) {
			for( const Move& move : moves ) {
				Node* child = new Node();
				child->move = move;
				child->openingName = openingName;
				children.push_back(child);
			}
		}


		Node* findChild(Move move) const {
			for( Node* child : children ) {
				if( child->move == move) return child;
			}
			return nullptr;
		}


		static Node* createRoot(const std::string& openingName, Move move) {
			Node* root = new Node();
			root->move = move;
			root->openingName = openingName;
			roots.push_back(root);
			return root;
		}
		

		static const std::vector<Node*>& getRoots() {
			return roots;
		}


	public:
		
		Move move;
		
		std::vector<Node*> children;
		
		std::string openingName;

		static inline std::vector<Node*> roots;
		
	};

	static void initTree() {
		
		Node* caroKannVienna = Node::createRoot("Caro-Kann / Vienna", { { 4, 1 }, { 4, 3 } }); //e4
		Node* caroKann = caroKannVienna->addChild({ { 2, 6 }, { 2,5 } }, "Caro-Kann");
		caroKann->addChildren({ { { 1, 0 }, { 2, 2 } }, { { 3, 1 }, { 3, 3 } } });
		caroKann->children[0]->addChild({ {3,6},{3,4} });
		caroKann->children[1]->addChild({ {3,6},{3,4} });

		Node* vienna = caroKannVienna->addChild({{4,6},{4,4}}, "Vienna");
		vienna->addChildren({ {{ 3, 1 }, { 3, 2 } },{{1,0},{2,2}} });

		Node* londonKingsIndian = Node::createRoot("London / King's Indian", { { 3, 1 }, { 3, 3 } }); //d4

		Node* london = londonKingsIndian->addChild({ {3,6},{3,4} }, "London");
		london->addChildren({ {{2,0},{5,3}} });
		london->children[0]->addChildren({ { {2,7},{5,4} },{{6,7},{5,5}} });
		london->children[0]->children[0]->addChild({ {6,0},{5,2} });
		london->children[0]->children[1]->addChild({ {6,0},{5,2} });

		Node* kingsIndian = londonKingsIndian->addChild({{6,6},{6,5}}, "King's Indian");
		kingsIndian->addChildren({ {{2,1},{2,3}},{{6,0},{5,2}} });
		kingsIndian->children[0]->addChildren({ { {5,7},{6,6} } });
		kingsIndian->children[1]->addChildren({ { {5,7},{6,6} } });
	}

}
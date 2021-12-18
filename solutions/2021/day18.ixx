module;
#include <cassert>
export module day18;

import parse;
import types;

import <array>;
import <string>;
import <utility>;
import <vector>;

struct Node {
	U64 val = 0;
	Node* left = nullptr;
	Node* right = nullptr;

};

static U64 node_buffer_tip = 0;
static Node node_buffer[1ULL << 15];

__forceinline Node* new_node() {
	assert(node_buffer_tip < 21000);
	Node* n = node_buffer + node_buffer_tip;
	node_buffer_tip++;
	n->val = 0;
	n->left = nullptr;
	n->right = nullptr;
	return n;
}

__forceinline void clear_node_buffer() {
	node_buffer_tip = 0;
}

static __forceinline Node* _parse(const std::string& line, U64& idx) {
	static std::string strbuf;

	if (line[idx] == '[') {
		auto* n = new_node();
		n->left = _parse(line, ++idx);
		assert(line[idx] == ',');
		n->right = _parse(line, ++idx);
		assert(line[idx] == ']');
		idx++;
		return n;
	} else if (std::isdigit(line[idx])) {
		auto* n = new_node();
		strbuf.clear();
		strbuf.push_back(line[idx++]);
		while (std::isdigit(line[idx])) {
			strbuf.push_back(line[idx++]);
		}
		n->val = std::stoull(strbuf);
		assert(line[idx] == ',' || line[idx] == ']');
		return n;
	}

	assert(false);
}

static Node* parse(const std::string& line) {
	U64 idx = 0;
	return _parse(line, idx);
}

static __forceinline U64 magnitude(const Node* const node) {
	if (!node->left && !node->right) return node->val;
	else return 3 * magnitude(node->left) + 2 * magnitude(node->right);
}

static __forceinline bool is_leaf(Node* node) {
	return node->left == nullptr && node->right == nullptr;
}

static void explode_left(Node* node, const std::vector<Node*>& history, U64 addval) {
	if (history.empty()) return;

	U64 parent_idx = history.size() - 1;
	Node* parent = history[parent_idx];
	while (true) {
		if (node == parent->right) {
			node = parent->left;
			while (!is_leaf(node)) node = node->right;
			node->val += addval;
			return;
		} else if (parent_idx == 0) {
			return;
		} else {
			node = parent;
			parent = history[--parent_idx];
		}
	}
}

static void explode_right(Node* node, const std::vector<Node*>& history, U64 addval) {
	if (history.empty()) return;

	U64 parent_idx = history.size() - 1;
	Node* parent = history[parent_idx];
	while (true) {
		if (node == parent->left) {
			node = parent->right;
			while (!is_leaf(node)) node = node->left;
			node->val += addval;
			return;
		} else if (parent_idx == 0) {
			return;
		} else {
			node = parent;
			parent = history[--parent_idx];
		}
	}
}

static __forceinline void _explode(Node* node, std::vector<Node*>& history, bool& done) {
	if (done) return;

	if (history.size() == 4 && node->left != nullptr) {
		assert(node->right != nullptr);
		assert(is_leaf(node->left) && is_leaf(node->right));

		explode_left(node, history, node->left->val);
		explode_right(node, history, node->right->val);

		node->left = nullptr;
		node->right = nullptr;
		node->val = 0;

		done = true;
		return;
	}

	history.push_back(node);
	if (node->left != nullptr) _explode(node->left, history, done);
	if (node->right != nullptr) _explode(node->right, history, done);
	history.pop_back();
}

static __forceinline Node* explode(Node* node) {
	static std::vector<Node*> history;
	history.clear();
	bool done = false;
	_explode(node, history, done);
	return done ? node : nullptr;
}

static __forceinline void _split(Node* node, std::vector<Node*>& history, bool& done) {
	if (done) return;

	if (is_leaf(node) && node->val >= 10) {
		auto* new_left = new_node();
		auto* new_right = new_node();
		new_left->val = node->val / 2;
		new_right->val = node->val / 2 + (node->val % 2 != 0);
		node->val = 0;
		node->left = new_left;
		node->right = new_right;
		done = true;
		return;
	}

	history.push_back(node);
	if (node->left != nullptr) _split(node->left, history, done);
	if (node->right != nullptr) _split(node->right, history, done);
	history.pop_back();
}

__forceinline Node* split(Node* node) {
	static std::vector<Node*> history;
	history.clear();
	bool done = false;
	_split(node, history, done);
	return done ? node : nullptr;
}

static __forceinline Node* reduce(Node* node) {
	while (true) {
		if (Node* new_node = explode(node); new_node != nullptr) {
			node = new_node;
			continue;
		} else if (Node* new_node = split(node); new_node != nullptr) {
			node = new_node;
			continue;
		} else break;
	}
	return node;
}

static __forceinline Node* add(Node* lexpr, Node* rexpr)
{
	Node* sum_node = new_node();
	sum_node->left = lexpr;
	sum_node->right = rexpr;
	return reduce(sum_node);
}

export struct Day18 {
	static constexpr U64 DAY_NUMBER = 18;
	static constexpr std::pair<U64, U64> SOLUTION = { 3411, 4680 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines("2021/18a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& snailfish)
	{
		std::pair<U64, U64> answer = { 0,0 };

		Node* root = parse(snailfish[0]);
		for (U64 i = 1; i < snailfish.size(); i++) {
			root = add(root, parse(snailfish[i]));
		}
		answer.first = magnitude(root);
		clear_node_buffer();

		U64 max_mag = 0;
		for (U64 i = 0; i < snailfish.size(); i++) {
			for (U64 j = 0; j < snailfish.size(); j++) {
				if (i == j) continue;
				Node* sum_node = add(parse(snailfish[i]), parse(snailfish[j]));
				max_mag = std::max(max_mag, magnitude(sum_node));
				clear_node_buffer();
			}
		}
		answer.second = max_mag;

		return answer;
	}
};


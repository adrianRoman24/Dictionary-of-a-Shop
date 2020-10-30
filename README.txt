# Dictionary-of-a-Shop

	This is an implementation of a dictionary that stores the products of a shop combining a self-balancing
search Tree and a linked list.

-createTree assigns a TTree structure and then assigns each element
from the structure with the corresponding one received as a parameter

-minimum returns the leftmost element of the root, if the root does not have a child left, returns the root

-maximum returns the rightmost element in the root, if roots-
does not have the right child, returns the root

-successor returns the leftmost element in the right subtree of
node given as a parameter, if it has no right subtree returns the first
grandfather who has as a child left on the first node of his parents

-predecessor returns the rightmost element in the left subtree of
node given as a parameter, if it does not have a left subtree returns the first grandfather who has as a child the right one on the first node of his parents

-rotateLeft puts it on the given node as a parameter instead of the child's left or right child

-rotateRight puts it on the given node as a parameter instead of the child's right or left child

-add_to_list adds a new node to the list corresponding to the given tree

-update_all_heights updates all node heights from current node to root (used for rotations)

-balanced is perhaps the most important function because it checks from the current node to the root if the shaft is balanced or if it requires a certain rotation at a certain node depending on the conditions of the avl shaft (shaft height st> shaft height dr with maximum 1)

-insert searches for where a node should be inserted based on the condition of a search tree (left <rad <dr). If it finds the node already inserted in the tree, it only adds it to the tree list, but if we find NULL it means that the node must be inserted on the left if node-> elem is smaller than the node to reach or on the right if node-> elem is larger than the node reached. Insert the node and check if the shaft needs to be balanced (use the balanced function)

-delete left / right / no_child delete the current node in the tree according to its children as the name suggests

-change_nodes changes two nodes in the tree and is used in deleteNode if a node meets both parents (changes node with successor
or and delete the node)

-delete initially looks for the node to be deleted and if it has
duplicates delete the last duplicate, and if it does not have duplicates it means that we are on one of the cases mentioned above

-buildTreesfromfile browses the given file as a parameter and adds the required elements in each tree

-each range function assigns a range structure and the initial index vector having an sg element, and along the way, if its capacity is exceeded, the vector with double capacity is reallocated

package edu.emory.mathcs.cs323.hw.autocomplete;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Queue;

import edu.emory.mathcs.cs323.trie.Trie;
import edu.emory.mathcs.cs323.trie.TrieNode;

public class HarmonAutocompleteExtraCredit<T> extends Trie<List<String>> implements IAutocomplete<List<String>> {
	private int k = 20;

	@Override
	public List<String> getCandidates(String prefix) {

		TrieNode<List<String>> n = find(prefix);
		if (n == null) {
			List<String> x = new ArrayList<String>();
			return x;
		}
		List<String> l = new ArrayList<>();
		List<String> l2 = n.getValue();
		if (l2 != null)
			for (int i = 0; i < k && i < l2.size(); i++)
				l.add(l2.get(i));
		if (l.size() == k)
			return l;
		// Breadth-first search to get candidates
		Queue<TrieNode> q = new ArrayDeque<TrieNode>();
		q.add(n);
		while (!q.isEmpty()) {
			TrieNode m = q.remove();
			if (m.isEndState()) {
				String w = getWord(m);
				if (!l.contains(w))
					l.add(w);
				if (l.size() == k)
					return l;
			}
			// Get map of children and add each Character key to List keys
			Map<Character, TrieNode<T>> childs = m.getChildrenMap();
			List<Character> keys = new ArrayList<>(childs.keySet());
			Collections.sort(keys);
			for (char key : keys)
				q.add(childs.get(key));
		}
		return l;
	}

	// Get full word from given node
	public String getWord(TrieNode c) {
		String s = "" + c.getKey();
		TrieNode p = c.getParent();
		while (p != null && p != getRoot()) {
			s = p.getKey() + s;
			p = p.getParent();
		}
		return s;
	}

	@Override
	public void pickCandidate(String prefix, String candidate) {
		TrieNode<List<String>> n = find(prefix);
		if (n == null) {
			put(prefix, new ArrayList<>());
			n = find(prefix);
			n.setEndState(false);
		}
		TrieNode<List<String>> m = find(candidate);
		if (m == null)
			put(candidate, null);
		else
			m.setEndState(true);
		List<String> v = n.getValue();
		if (v == null) {
			v = new ArrayList<String>();
			n.setValue(v);
		}
		v.remove(candidate);
		v.add(0, candidate);
	}
}
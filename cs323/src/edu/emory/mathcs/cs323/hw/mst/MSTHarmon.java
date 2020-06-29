/** THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Wesley Harmon **/

// Test!

package edu.emory.mathcs.cs323.hw.mst;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Set;

import edu.emory.mathcs.cs323.graph.Edge;
import edu.emory.mathcs.cs323.graph.Graph;
import edu.emory.mathcs.cs323.graph.span.MSTAll;
import edu.emory.mathcs.cs323.graph.span.SpanningTree;

/**
 * @author Wesley Harmon ({@code wharmon@emory.edu})
 */
public class MSTHarmon implements MSTAll {
	private Edge edge;
	List<SpanningTree> list;

	@Override
	public List<SpanningTree> getMinimumSpanningTrees(Graph graph) {
		list = new ArrayList<>();
		if (graph.size() <= 1)
			return list;
		for (int i = 0; i < graph.size(); i++) {
			SpanningTree tree = new SpanningTree();
			PriorityQueue<Edge> queue = new PriorityQueue<>();
			Set<Integer> visited = new HashSet<>();

			List<Edge> edges = graph.getIncomingEdges(i);
			Edge min = edges.get(0);
			for (Edge edge : edges)
				if (edge.compareTo(min) < 0)
					min = edge;

			tree.addEdge(min);
			add(graph, queue, visited, tree, i);
			if (i + 1 < graph.size() - 1)
				add(graph, queue, visited, tree, i + 1);

			getMinimumSpanningTrees(graph, list, queue, visited, tree);
			// if (graph.size()<=2) break;
		}

		/*
		 * Thank god for this friking method in here Jesus
		 */
		String[] l = new String[list.size()];
		for (int i = 0; i < list.size(); i++) {
			l[i] = list.get(i).getUndirectedSequence();
			for (int j = 0; j < list.size(); j++) {
				if (i == j)
					continue;
				else
					l[j] = list.get(j).getUndirectedSequence();

				if (l[j].equals(l[i]))
					list.remove(j);
			}
		}
		
		return list;
	}

	/**
	 * Recursively finds all minimum spanning trees of given graph
	 * 
	 * @param graph
	 *            an undirected graph containing zero to many spanning trees.
	 * @param list
	 *            List of minimum spanning trees of given graph
	 * @param queue
	 *            Queue of all edges awaited to explore
	 * @param visited
	 *            Set of visited vertices
	 * @param tree
	 *            A minimum spanning tree in the making
	 * @return a list of minimum spanning trees
	 */
	public List<SpanningTree> getMinimumSpanningTrees(Graph graph, List<SpanningTree> list, PriorityQueue<Edge> queue,
			Set<Integer> visited, SpanningTree tree) {
		if (list.size() < 1)
			;
		else if (isTooHeavy(tree, list.get(0)))
			return list;

		if (isSpanningTree(tree, graph.size())) {
			list.add(tree);
			return list;
		}

		theRemoval(graph, queue, visited, tree);

		if (queue.isEmpty())
			return list;

		while (!queue.isEmpty()) {
			edge = queue.poll();

			PriorityQueue<Edge> cqueue = new PriorityQueue<>(queue);
			SpanningTree ctree = new SpanningTree(tree);
			Set<Integer> cvisited = new HashSet<>(visited);

			ctree.addEdge(edge);
			add(graph, cqueue, cvisited, ctree, edge.getSource());
			// add(graph, cqueue, cvisited, ctree, edge.getTarget());

			getMinimumSpanningTrees(graph, list, cqueue, cvisited, ctree);

			if (cqueue.peek().compareTo(edge) != 0)
				break;
		}

		return list;
	}

	/**
	 * @return true if the weight of a given tree is greater than that of the
	 *         minimum spanning tree
	 */
	public boolean isTooHeavy(SpanningTree tree, SpanningTree mst) {
		if (tree.compareTo(mst) == 1)
			return true;
		return false;
	}

	/**
	 * @return true if the given tree is a spanning tree
	 */
	public boolean isSpanningTree(SpanningTree tree, int gsize) {
		if (tree.size() + 1 == gsize)
			return true;
		return false;
	}

	/**
	 * Add the given edge to the copied tree, the target vertex to the copied
	 * set, and the incoming edges to the copied queue
	 * 
	 * @param queue
	 *            Queue of all vertices awaited to explore
	 * @param visited
	 *            Set of visited vertices
	 * @param graph
	 *            Graph
	 * @param target
	 *            Target vertex
	 */
	private void add(Graph graph, PriorityQueue<Edge> queue, Set<Integer> visited, SpanningTree tree, int target) {
		visited.add(target);
		for (Edge edge : graph.getIncomingEdges(target)) {
			if (!visited.contains(edge.getSource()))
				queue.add(edge);
		}
	}

	/**
	 * The most frustrating part: remove ineligible edges from the queue
	 * 
	 * @param queue
	 *            Queue of all vertices awaited to explore
	 * @param visited
	 *            Set of visited vertices
	 * @param graph
	 *            Graph
	 * @param tree
	 *            Current mst in the making
	 */
	public void theRemoval(Graph graph, PriorityQueue<Edge> queue, Set<Integer> visited, SpanningTree tree) {
		for (Integer i : visited) {
			for (Edge e : graph.getIncomingEdges(i)) {
				if (visited.contains(e.getTarget()) && tree.getEdges().contains(e))
					queue.remove(e);
			}
		}
	}

}

package edu.emory.mathcs.cs323.hw.mst;

import java.util.ArrayList;
import java.util.List;

import edu.emory.mathcs.cs323.graph.Graph;
import edu.emory.mathcs.cs323.graph.span.MSTAll;
import edu.emory.mathcs.cs323.graph.span.SpanningTree;

public class MSTHarmonTest {
	public static void main(String[] args) {
		// MSTAlgorithm prim = new MSTPrim();
		// SpanningTree prim2a = prim.getMinimumSpanningTree(getGraph2a());
		// SpanningTree prim3b = prim.getMinimumSpanningTree(getGraph3b());
		//
		// System.out.println(getGraph1().toString());
		//
		// System.out.println(prim3b.toString());

		test();
	}

	public static void test() {
		List<Graph> graphs = new ArrayList<>();
		graphs.add(getGraph1());
		graphs.add(getGraph2a());
		graphs.add(getGraph3a());
		graphs.add(getGraph3b()); // 2 solutions

		Graph graph;

		MSTAll har = new MSTHarmon();
		List<SpanningTree> me3b = har.getMinimumSpanningTrees(getGraph3b());

		for (SpanningTree mst : me3b) {
			String s = mst.toString();
			System.out.println(s);
		}

		StringBuilder test = new StringBuilder();
		test.append("0 <- 1 : 1.000000 \n");
		test.append("0 <- 2 : 2.000000 \n");
		test.append("\n");
		test.append("1 <- 0 : 1.000000 \n");
		test.append("1 <- 2 : 2.000000 \n");

		// System.out.println("Test:\n"+test);

	}

	static Graph getGraph1() {
		return new Graph(1);
	}

	static Graph getGraph2a() {
		Graph graph = new Graph(2);
		return graph;
	}

	static Graph getGraph3a() {
		Graph graph = new Graph(3);
		graph.setUndirectedEdge(0, 1, 1);
		graph.setUndirectedEdge(0, 2, 2);
		graph.setUndirectedEdge(1, 2, 3);
		return graph;
	}

	// Two solutions!
	static Graph getGraph3b() {
		Graph graph = new Graph(3);
		graph.setUndirectedEdge(0, 1, 1);
		graph.setUndirectedEdge(0, 2, 2);
		graph.setUndirectedEdge(1, 2, 2);
		return graph;
	}
}

package edu.emory.mathcs.cs323.hw.mst;

import java.util.ArrayList;
import java.util.List;

import edu.emory.mathcs.cs323.graph.Edge;
import edu.emory.mathcs.cs323.graph.Graph;
import edu.emory.mathcs.cs323.graph.span.MSTAll;
import edu.emory.mathcs.cs323.graph.span.SpanningTree;

public class MSTSol implements MSTAll {
	@Override
	public List<SpanningTree> getMinimumSpanningTrees(Graph graph) {
		List<SpanningTree> list = new ArrayList<>();
		double[][] a = new double[graph.size()][graph.size()];
		List<Edge> all = graph.getAllEdges();
		for (int i = 0; i < graph.size(); i++)
			a[i][i] = 0;

		for (Edge edge : all) {
			int x = edge.getSource();
			int y = edge.getTarget();
			a[x][y] = edge.getWeight();
			a[y][x] = edge.getWeight();
		}

		System.out.println(a[0][0]);
		double min = 100;
		for (int i = 0; i < graph.size(); i++) {
			for (int j = 0; j < graph.size(); j++) {
				if (i == j)
					continue;
				double temp = min;
				if (a[i][j] < min)
					min = a[i][j];

			}

		}

		return list;
	}

}

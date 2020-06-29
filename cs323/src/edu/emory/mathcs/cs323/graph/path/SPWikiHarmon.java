/**
 * Copyright 2015, Emory University
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package edu.emory.mathcs.cs323.graph.path;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import edu.emory.mathcs.cs323.graph.Graph;

public class SPWikiHarmon 
{
	List<String> titles;
	int[][] links;
	Integer[] path;

	public SPWikiHarmon(InputStream inTitles, InputStream inLinks) throws Exception 
	{
		titles = getTitles(inTitles);
		links = getLinks(inLinks, titles.size());

		Graph g = new Graph(titles.size());
		Dijkstra d = new Dijkstra();
		int source = 0;
		int target = 0;
		int[] sources = {4590, 2809, 1946, 1877, 534, 563, 219, 356, 3700, 2202};
		int[] targets = {18, 2801, 1947, 1233, 550, 60, 2, 4002, 1826, 967};
		int size = 10; // Number of pairs
		
		Map<Integer, String> map = new HashMap<>();
		
		for (int i=0; i<titles.size(); i++)
		{
			map.put(i, titles.get(i));
			
			for (int j=0; j<links[i].length; j++)
				g.setDirectedEdge(i, links[i][j], 1);
		}
		
		StringBuilder sb = new StringBuilder();
		for (int i=0; i<size; i++)
		{
			source = sources[i];
			target = targets[i];
			path = d.getShortestPath(g, source, target);
			
			int j = source;
			do
			{
				sb.append(map.get(j)+" -> ");
				j = path[j];
			}
			while (j != target);
			
			sb.append(map.get(j));
			System.out.printf("Shortest Path %d: '%s' (%d) to '%s' (%d):\n", i+1, map.get(source), source, map.get(target), target);
			System.out.println(sb+"\n");
			sb = new StringBuilder();
		}
	}
	
	

	public List<String> getTitles(InputStream in) throws Exception 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		List<String> list = new ArrayList<>();
		String line;

		while ((line = reader.readLine()) != null)
			list.add(line.trim());

		reader.close();
		return list;
	}

	public int[][] getLinks(InputStream in, int size) throws Exception 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		Pattern p = Pattern.compile(" ");
		int[][] array = new int[size][];
		int[] links;
		String line;
		String[] t;
		int i, j;

		for (i = 0; (line = reader.readLine()) != null; i++) 
		{
			line = line.trim();

			if (line.isEmpty())
				array[i] = new int[0];
			else 
			{
				t = p.split(line);
				links = new int[t.length];
				array[i] = links;

				for (j = 0; j < links.length; j++)
					links[j] = Integer.parseInt(t[j]);
			}
		}

		return array;
	}

	static public void main(String[] args) throws Exception 
	{
		final String TITLE_FILE = "/media/wharmon/MyStuff/School/cs323/assignments/hw4/wiki-titles-small.txt";
		final String LINK_FILE = "/media/wharmon/MyStuff/School/cs323/assignments/hw4/wiki-links-small.txt";
		new SPWikiHarmon(new FileInputStream(TITLE_FILE), new FileInputStream(LINK_FILE));
	}
}

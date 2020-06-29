package edu.emory.mathcs.cs323.queue;

import java.util.PriorityQueue;

public class FindIndex 
{
	static int indexOf(int[] heap, int key)
	{
		PriorityQueue<Integer> q = new PriorityQueue<>();
		q.add(1);
		while (!q.isEmpty())
		{
			int ind = q.remove();
			if (ind >= heap.length) continue;
			if (key > heap[ind]) continue;
			if (heap[ind]==key) return ind;
			q.add(2*ind);
			q.add(2*ind+1);
		}
		return -1;
	}
	
	public static void main(String[] args) 
	{
		int[] bin = {0,7,6,5,4,3,1,2};
		int ans = indexOf(bin, 8);
		System.out.println(ans);

	}

}

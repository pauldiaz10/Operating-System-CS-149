/*Group #3
Group Members:
Yulong Chen (ID: 0101474984)
Paul Diaz (ID: 010188451)
Haokai Ma (ID: 010473164)
Rong Wang (ID: 010359492)
Long Trinh (ID: 010174476)
*/

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Random;

public class SwappingAndPaging {

	private static int JOBSIZE = 150;			//150 is number of process we use
	private static int MEMORYSIZE = 100;		//100 MB size of maim memory
	private static char [] ProcessName = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O'};
	private static int MAXTIME = 600;

	/**
	 * Method that calls the method runSimulation
	 */
	public static void main(String[] args) {
		RunSimulation();
	}
	/**
	 * Private Utility Method that actually runs the simulation
	 */
	private static void RunSimulation () {
		
		int[] hitArr = new int[5];
		int[] missArr = new int[5];
		int[] swapIn = new int[5];
		LinkedList<Process> JobQueueInitial = JobGenerator();
		List<ReplacementAlgorithm> algorithms = new ArrayList<>();
		algorithms.add(new FirstInFirstOut("FIFO"));
		algorithms.add(new LeastRecentlyUsed("LRU"));
		algorithms.add(new LeastFrequentlyUsed("LFU"));
		algorithms.add(new MostFrequentlyUsed("MFU"));
		algorithms.add(new RandomPick("RandomPick"));
		
		
		for (int m = 0 ; m < algorithms.size(); m++) {
		
		//Deep copy
		LinkedList<Process> JobQueue = new LinkedList<>();
		for (int i = 0 ; i < JobQueueInitial.size(); i++) {
			int arrivalTime = JobQueueInitial.get(i).getArrivalTime();
			int size = JobQueueInitial.get(i).getSize();
			double durations = JobQueueInitial.get(i).getDurations();
			Process temp = new Process(arrivalTime, size, durations);
			temp.setName(JobQueueInitial.get(i).getName());
			JobQueue.add(temp);
		}
		ReplacementAlgorithm a = algorithms.get(m);
		System.out.printf("\n\n\n\n\n\n----------------Running %s Algorithm----------------\n", a.getName());
		LinkedList<Process> RunningQueue = new LinkedList<>();
		
		LinkedList<Page> replacementList = new LinkedList<>();
		LinkedList<Page> PageQueue = new LinkedList<>();
		
		for (int i = 0 ; i < MEMORYSIZE; i++) {
			replacementList.add(new Page(-1, "."));
		}
		int miss = 0;
		int hit = 0;
		int swap = 0;
		int freeList = MEMORYSIZE;
		int timeSlice = 0;
		
		while (timeSlice <= MAXTIME ) {
			while (freeList >= 4 && JobQueue.size() > 0 && JobQueue.peek().getArrivalTime()*10 <= timeSlice) {
				swap++;
				Process p = JobQueue.poll();
				RunningQueue.add(p);
				printJobs(timeSlice, p, replacementList);
			}
			for (int i = 0 ; i < RunningQueue.size() ; i++) {
				
				Process p = RunningQueue.get(i);
				boolean exist = false;
				
				for (int j = 0 ; j < replacementList.size() ; j++) {
					
					if (replacementList.get(j)._ProcessName.equals(p.getName())&&
							replacementList.get(j)._PageNumber == p.getCurrentReferences())
						{
							hit++;
							exist = true;
							System.out.println("Hit!");
							replacementList.get(j).addReferencedTime();
							if (a.getName().equals("LRU")) {
								PageQueue.remove(replacementList.get(j));
								PageQueue.add(replacementList.get(j));
							}
							printPages(timeSlice, p, p.getCurrentReferences(), replacementList, j+1, null);
							break;
						}
					
				}
				
				if (!exist) {
					String evicted = null;
					miss++;
					if (freeList == 0) {
						evicted = a.run(replacementList, PageQueue);
						System.out.printf("\n----------%s-------------\n", a.getName());
						freeList++;
					} 
					for (int k = 0; k <replacementList.size(); k++) {
						if (replacementList.get(k)._PageNumber == -1 &&
							replacementList.get(k)._ProcessName.equals(".")) {
							Page temp = new Page(p.getCurrentReferences(), p.getName());
							temp.addReferencedTime();
							replacementList.set(k, temp);
							PageQueue.add(temp);
							printPages(timeSlice, p, p.getCurrentReferences(), replacementList, k+1, evicted);
							break;
						}
					}
					freeList--;
					
				
					
				}
				p.setCurrentReferences(GetNextReferences(p));
				
				p.setDurations(p.getDurations() - 0.1);
				
				if (p.getDurations() <= 0) {
					int count = 0;
					p.setDurations(0);
					RunningQueue.remove(i);
					
					
					
					for (int k = 0; k < replacementList.size(); k++) {
						if (replacementList.get(k)._ProcessName.equals(p.getName()))  {
							PageQueue.remove(replacementList.get(k));
							replacementList.set(k, new Page(-1, "."));
							count++;
						}
						
					}
					freeList += count;
					printJobs(timeSlice, p, replacementList);
					}
				
				

			}
			timeSlice++;
		}
		System.out.println("\nHit: " + hit);
		System.out.println("\nMiss: " + miss);
		hitArr[m] = hit;
		missArr[m] = miss;
		swapIn[m] = swap;
		}
		
		printResult(hitArr, missArr, swapIn);
	}
	/**
	 * Private Utility Method that calculate Hit, Miss, Hit ratio, Total Swapped-in
	 * in different all algorithms
	 * @param hitArr is an array of all hits
	 * @param missArr is an aray of all misses
	 * @param swapIn is an array of all swapped-in happended
	*/
	private static void printResult(int[] hitArr, int[] missArr, int[] swapIn) {
		
		double averageRatio = 0;
		int averageHit = 0;
		int averageMiss = 0;
		int averageSwapIn = 0;
		String[] name = {"First In First Out", "Least Recently Used", "Least Frequently Used",
				"Most Frequently Used", "Random Pick"};
		
		System.out.println("------------------------------------------------------");
		System.out.println("----------------------RESULT--------------------------");
		for (int i = 0 ; i < name.length ; i++) {
		double ratio = (double)hitArr[i] / (hitArr[i] + missArr[i]);
		averageRatio += ratio;
		averageHit += hitArr[i];
		averageMiss += missArr[i];
		averageSwapIn += swapIn[i];
		System.out.println(name[i]);
		System.out.println("Hit: " + hitArr[i]);
		System.out.println("Miss: " + missArr[i]);
		System.out.println("Hit ratio: " + ratio);
		System.out.println("Total Swapped-in: " + swapIn[i]);
		System.out.println("------------------------------------------------------");
		}
		
		System.out.println("Average hit over 5 runs: " + averageHit / 5);
		System.out.println("Average miss over 5 runs: " + averageMiss / 5);
		System.out.println("Average hit ratio over 5 runs: " + averageRatio / 5);
		System.out.println("Average swap in over 5 runs: " + averageSwapIn / 5);
		
	}
	/**
     * Private Utility method that returns the next process reference
     * @param p is the main process where we find the next reference
     * @return the next process reference number
    */
	private static int GetNextReferences(Process p) {
		
		int nextReferences = 0;
		Random rand = new Random(System.currentTimeMillis());
		int r = rand.nextInt(p.getSize());
		// 0 <= r < 70%
		if (0 <= r && r < (p.getSize() - 1) * 0.7) {
			
			nextReferences = p.getCurrentReferences() + rand.nextInt(3) - 1;
			
		} else {
			nextReferences = p.getCurrentReferences() + rand.nextInt(p.getSize() - 1) + 2;
		}
		
		if (nextReferences < 0) {
			nextReferences = p.getSize() + nextReferences + 1;
		} else {
			nextReferences = nextReferences % p.getSize();
		}
		
		return nextReferences;
		
	}
	/**
     * Private Utility Method that prints all the process, including the names, 
     * arrival time, duration time
     * @param timeSlice is the time given to a process
     * @param p is the process to be printed
     * @param map is actually the memory list of processes
     */
	private static void printJobs(int timeSlice, Process p, LinkedList<Page> map) {
		if (p.getDurations() > 0){
			System.out.printf("[%d] Process name: %s, Size: %d, Enter: %d, Duration: %d\n",
					timeSlice/10, p.getName(), p.getSize(), timeSlice/10, (int)p.getDurations());
		} else {
			System.out.printf("[%d] Process name: %s, Size: %d, Exit: %d, Duration: %d\n",
					timeSlice/10, p.getName(), p.getSize(), timeSlice/10, (int)p.getDurations());
		}
		printMemoryMap(map);
	}
	/**
     * Private Utility method that prints all the pages in the memory, including the process names,
     * page reference number, page-in memory
     * @param timeSlice is the time given to a process
     * @param referecencedPage is the number of a referenced page
     */
	private static void printPages(int timeSlice, Process p, int referencedPage, LinkedList<Page> map, int location, String evicted) {
		if (evicted == null){
			System.out.printf("[%d] Process name: %s: Page-referenced: %d, Page-in-memory: %d",
					timeSlice/10, p.getName(), referencedPage, location );
		} else {
			System.out.printf("[%d] Process name: %s: Page-referenced: %d, Page-in-memory: %d, Page will be evicted: %s", 
					timeSlice/10, p.getName(), referencedPage, location, evicted );
		}
		System.out.println();
		printMemoryMap(map);
	}
	/**
     * Private utility method that prints memory map
     * @param map is memory list of processes
    */
	private static void printMemoryMap (LinkedList<Page> map) {
		
		System.out.println("Mmemory Map: ");
		for (int i = 0; i < map.size() ; i++) {
			System.out.print(map.get(i)._ProcessName + " ");
		}
		System.out.println();
		
	}
    /**
     * Private Utility Method that generates process jobs
     * @return in Linked list data structure of process jobs
     */
	private static LinkedList<Process> JobGenerator() {

		LinkedList<Process> JobQueue = new LinkedList<>();
		
		for (int i = 0; i < JOBSIZE; i++) {
			Random rand = new Random();
			int arrivalTime = rand.nextInt(61); // generate numbers between 0 to 60;
			int size = SizeGenerator();
			double durations = rand.nextInt(5) + 1;
			Process p = new Process(arrivalTime, size, durations);
			JobQueue.add(p);
		}
		
		Collections.sort(JobQueue, new Comparator<Process>() {

			public int compare(Process a, Process b) {
				if (a.getArrivalTime() < b.getArrivalTime()) {
					return -1;
				} else {
					return 1;
				}
			}

		});
		int count = 0;
		for (char c : ProcessName) {
			for (int i = 0; i <= 9; i++) {
				JobQueue.get(count).setName("" + c + i);
				count++;
			}
		}
		return JobQueue;
	}
	/**
     * Private Utility Method that generates the size of a process
     * @return the size given to a process
     */
	private static int SizeGenerator() {

		Random rand = new Random();

		int num = rand.nextInt(4);
		if (num == 1) {
			return 5;
		} else if (num == 2) {
			return 11;
		} else if (num == 3) {
			return 17;
		} else {
			return 31;
		}

	}

}

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Random;

public class SchedulingAlgorithms {
	public static int MAX_QUANTA = 100;
	public static int MAX_PROCESSES = 50;

	public static void runSchedule(Schedule alg, ArrayList<Process> a) {
		System.out.println("\nTimechart:");
		System.out.println(alg.schedule(a));
		System.out.println("\nAverage Turnaround Time: " + alg.averageTurnaroundTime());
		System.out.println("Average Waiting Time: " + alg.averageWaitingTime());
		System.out.println("Average Response Time: " + alg.averageResponseTime());
		System.out.println("Total Throughput: " + alg.totalThroughput());

	}

	public static ArrayList<Process> generate(int count, int seed) {
		ArrayList<Process> r = new ArrayList<>();
		Random rand = new Random(seed); // Sometimes you want the same shit

		for (int i = 0; i < count; i++) {
			// public Process(String name, float arrivalTime, float
			// expectedTime, int priority){
			float arrival = (float) (rand.nextDouble() * 99.0);
			float run = (float) (rand.nextDouble() * 9.9 + 0.1);
			int priority = rand.nextInt(4) + 1;
			Process n = new Process("", arrival, run, priority);
			r.add(n);
		}
		r.sort(new Comparator<Process>() {

			public int compare(Process a, Process b) {
				if (a.getTimeArrival() < b.getTimeArrival()) {
					return -1;
				} else {
					return 1;
				}
			}

		});
		for (int i = 0; i < count; i++) {
			r.get(i).setId(GenerateProcessId.toAlphabetic(i));
		}
		return r;
	}

	public static void runSimulation(int seed){
		System.out.println("Simulation #" + seed);
		System.out.println("\nProcesses: ");
		ArrayList<Process> a = generate(MAX_PROCESSES, seed);
		for (int i = 0; i < a.size(); i++) {
			System.out.println(a.get(i).toString());
		}
		
		Schedule FCFS = new FirstComeFirstServed();
		Schedule SJF = new ShortestJobFirst();
		Schedule SRT = new ShortestRemainingTime();
		Schedule RR = new RoundRobin();
		Schedule HPF_NP = new HighestPriorityFirst();
		((HighestPriorityFirst) HPF_NP).setPreemptive(false);
		((HighestPriorityFirst) HPF_NP).setAging(false);
		
		Schedule HPF_P = new HighestPriorityFirst();
		((HighestPriorityFirst) HPF_P).setPreemptive(true);
		((HighestPriorityFirst) HPF_P).setAging(false);
		
		Schedule HPF_NP_AGING = new HighestPriorityFirst();
		((HighestPriorityFirst) HPF_NP_AGING).setPreemptive(false);
		((HighestPriorityFirst) HPF_NP_AGING).setAging(true);
		
		Schedule HPF_P_AGING = new HighestPriorityFirst();
		((HighestPriorityFirst) HPF_P_AGING).setPreemptive(true);
		((HighestPriorityFirst) HPF_P_AGING).setAging(true);
		
		System.out.println("\nFirst Come First Served Algorithm");
		runSchedule(FCFS, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nShortest Job First Algorithm");
		runSchedule(SJF, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nShortest Remaining Time Algorithm");
		runSchedule(SRT, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nRound Robin Algorithm");
		runSchedule(RR, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nHighest Priority First (Non-Preemptive)");
		runSchedule(HPF_NP, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nHighest Priority First (Preemptive)");
		runSchedule(HPF_P, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nHighest Priority First (Non-Preemptive, Aging)");
		runSchedule(HPF_NP_AGING, generate(MAX_PROCESSES, seed));
		
		System.out.println("\n\nHighest Priority First (Preemptive, Aging)");
		runSchedule(HPF_P_AGING, generate(MAX_PROCESSES, seed));
		
		System.out.print("\n\n\n");

	}
	public static void main(String[] args) {
		for(int i=1; i<=5; i++)
			runSimulation(i);

	}
}

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class ShortestRemainingTime implements Schedule {
	private float timeTurnaround;
	private float timeWaiting;
	private float timeResponse;
	private int throughputTotal;

	public String schedule(ArrayList<Process> a) {
		int quantum = 0;
		StringBuilder sb = new StringBuilder();

		boolean lastProcess = true;
		boolean running = true;

		ArrayList<Process> q = new ArrayList<>();
		ArrayList<Process> d = new ArrayList<>();

		while (running && !(q.isEmpty() && a.isEmpty())) {
			// if process Queue is empty, process Array List is NOT empty, and
			// open for service
			if (q.isEmpty() && quantum < SchedulingAlgorithms.MAX_QUANTA - 1 && !a.isEmpty()) {
				q.add(a.remove(0));
				while (q.get(0).getTimeArrival() > quantum) {
					quantum++;
					sb.append("- ");
				}
			}

			// make sure anything that has arrived is put into q
			if (quantum < SchedulingAlgorithms.MAX_QUANTA - 1) {
				while (!a.isEmpty() && a.get(0).getTimeArrival() < quantum) {
					q.add(a.remove(0));
				}
			}
			// This is not repeating code. previous lines finish program if the
			// program is not fed enough processes to finish.
			while (!a.isEmpty() && a.get(0).getTimeArrival() < quantum) {
				q.add(a.remove(0));
			}

			// Sort q based on expected time
			Collections.sort(q, new Comparator<Process>() {
				@Override
				public int compare(Process process1, Process process2) {
					if (process1.getTotalExpectedRun() < process2.getTotalExpectedRun()) {
						return -1;
					}
					if (process1.getTotalExpectedRun() > process2.getTotalExpectedRun()) {
						return 1;
					}
					return 0;
				}
			});

			// if the last process ended, start latest process
			// starts out as true for first iteration
			if (!q.isEmpty() && lastProcess) {
				if (!q.get(0).isStartedProcess()) {
					q.get(0).setTimeStart(quantum);
				}
				lastProcess = false;
			}

			// run process
			if (!q.isEmpty()) {
				q.get(0).runProcess();// timeRemaining -= 1;

				if (quantum < SchedulingAlgorithms.MAX_QUANTA - 1) {// If the simulation is
													// active
					if (!q.isEmpty() && !q.get(0).isStartedProcess()) {
						q.get(0).setTimeStart(quantum);
					}

					if (q.get(0).getTimeRemaining() > 0) {
						if ((quantum % 10) == 0) {
							sb.append(("\n" + q.get(0).getId()));
						} else {
							sb.append(q.get(0).getId());
						}
					} else if (q.get(0).getTimeRemaining() < 0) {
						if ((quantum % 10) == 0) {// regarding formatting
							sb.append(("\n" + q.get(0).getId()));
						} else {
							sb.append(q.get(0).getId());
						}
						q.get(0).setTimeFinish(quantum + 1);
						lastProcess = true;
						d.add(q.remove(0));
					}
				} else if (quantum >= SchedulingAlgorithms.MAX_QUANTA - 1) {// Simulation
															// becomes inactive,
															// does not accept
															// new processes
					lastProcess = false;
					if (q.get(0).isStartedProcess()) {
						if (q.get(0).getTimeRemaining() < 0) {
							if ((quantum % 10) == 0) {
								sb.append(("\n" + q.get(0).getId()));
							} else {
								sb.append(q.get(0).getId());
							}
							q.get(0).setTimeFinish(quantum + 1);
							d.add(q.remove(0));
						} else if (q.get(0).getTimeRemaining() > 0) {
							if ((quantum % 10) == 0) {
								sb.append(("\n" + q.get(0).getId()));
							} else {
								sb.append(q.get(0).getId());
							}
						}
					} else if (!q.get(0).isStartedProcess()) {
						running = false;
					} else {
						running = false;
					}
				}
			}
			quantum++;
		}
		int processes = d.size();
		double waiting = 0;
		for (int i = 0; i < processes; i++) {
			waiting = d.get(i).getWaiting();
			timeTurnaround += d.get(i).getTurnaround();
			timeWaiting += waiting;
			timeResponse += d.get(i).getResponse();

		}

		timeTurnaround = timeTurnaround / processes;
		timeWaiting = timeWaiting / processes;
		timeResponse = timeResponse / processes;
		throughputTotal = processes;

		return sb.toString();
	}

	public float averageTurnaroundTime() {
		return timeTurnaround;
	}

	public float averageWaitingTime() {
		return timeWaiting;
	}

	public float averageResponseTime() {
		return timeResponse;
	}

	public float totalThroughput() {
		return throughputTotal;
	}
}

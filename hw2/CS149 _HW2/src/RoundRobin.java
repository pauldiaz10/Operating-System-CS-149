import java.util.ArrayList;

public class RoundRobin implements Schedule {
	private float timeTurnaround;
	private float timeWaiting;
	private float timeResponse;
	private int throughputTotal;

	public String schedule(ArrayList<Process> a) {
		StringBuilder sb = new StringBuilder();
		int quantum = 0;

		boolean first = true;
		boolean running = true;
		Process temp;

		ArrayList<Process> q = new ArrayList<>();
		ArrayList<Process> d = new ArrayList<>();

		while (running && !(q.isEmpty() && a.isEmpty())) {

			if (q.isEmpty() && !a.isEmpty() && quantum < 99) {
				q.add(a.remove(0));
				while (q.get(0).getTimeArrival() > quantum) {
					quantum++;
					sb.append("- ");
				}
			}

			if (quantum < 99) {
				while (!a.isEmpty() && a.get(0).getTimeArrival() < quantum) {
					q.add(a.remove(0));
				}
			}

			while (!a.isEmpty() && a.get(0).getTimeArrival() < quantum) {
				q.add(a.remove(0));
			}

			if (!q.isEmpty() && first) {
				if (!q.get(0).isStartedProcess()) {
					q.get(0).setTimeStart(quantum);
				}
				first = false;
			}

			if (!q.isEmpty()) {
				q.get(0).runProcess();

				if (quantum < 99) {
					if (!q.get(0).isStartedProcess()) {
						q.get(0).setTimeStart(quantum);
					}

					if (q.get(0).getTimeRemaining() > 0) {
						if ((quantum % 10) == 0) {
							sb.append(("\n" + q.get(0).getId()));
						} else {
							sb.append(q.get(0).getId());
						}
						temp = q.remove(0);
						q.add(temp);

					} else if (q.get(0).getTimeRemaining() < 0) {
						if ((quantum % 10) == 0) {
							sb.append(("\n" + q.get(0).getId()));
						} else {
							sb.append(q.get(0).getId());
						}
						q.get(0).setTimeFinish(quantum + 1);

						temp = q.remove(0);
						d.add(temp);
					}
				} else if (quantum >= 99) {
					while (!q.isEmpty() && !q.get(0).isStartedProcess()) {
						q.remove(0);
					}

					if (!q.isEmpty() && q.get(0).isStartedProcess()) {
						if (q.get(0).getTimeRemaining() < 0) {
							if ((quantum % 10) == 0) {
								sb.append(("\n" + q.get(0).getId()));
							} else {
								sb.append(q.get(0).getId());
							}
							q.get(0).setTimeFinish(quantum + 1);
							temp = q.remove(0);
							d.add(temp);
						} else if (q.get(0).getTimeRemaining() > 0) {
							if ((quantum % 10) == 0) {
								sb.append(("\n" + q.get(0).getId()));
							} else {
								sb.append(q.get(0).getId());
							}
							temp = q.remove(0);
							q.add(temp);
						}
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

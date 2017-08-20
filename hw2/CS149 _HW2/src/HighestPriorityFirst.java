import java.util.ArrayList;

public class HighestPriorityFirst implements Schedule {
	private float timeTurnaround;
	private float timeWaiting;
	private float timeResponse;
	private int throughputTotal;
	private boolean preemptive;
	private boolean aging;
	private ArrayList<Process> P1;
	private ArrayList<Process> P2;
	private ArrayList<Process> P3;
	private ArrayList<Process> P4;


	public String schedule(ArrayList<Process> a) {
		ArrayList<Process> d = new ArrayList<>();

		P1 = new ArrayList<>();
		P2 = new ArrayList<>();
		P3 = new ArrayList<>();
		P4 = new ArrayList<>();

		Process next = null;
		Process current = null;
		int process = 0;
		int quantum = 0;

		StringBuilder sb = new StringBuilder();

		while (quantum < SchedulingAlgorithms.MAX_QUANTA) {
			if (next == null && process < a.size()) {
				next = a.get(process++);
			}
			while (next != null && next.getTimeArrival() <= quantum) {
				add(next, quantum);

				next.setTimeToFinish(next.getTotalExpectedRun());

				if (process < a.size() - 1) {
					next = a.get(process++);
				} else {
					next = null;
				}
			}

			if (preemptive) {

				current = select(current, quantum);

				if (current != null) {

					if (!current.isStarted()) {
						current.setStarted(true);
						current.setTimeStart(quantum);

					}

					sb.append(current.getId());

					float timeToFinish = current.getTimeToFinish() - 1;
					if (timeToFinish <= 0) {
						timeToFinish = 0;
					}

					current.setTimeToFinish(timeToFinish);

					if (timeToFinish == 0) {

						current.setTimeFinish(quantum);

						remove(current);

						d.add(current);

						current = null;
					}
				}

				quantum++;
			} else {

				current = select(current, quantum);

				if (current != null) {

					if (!current.isStarted()) {
						current.setStarted(true);
						current.setTimeStart(quantum);

					}

					while (quantum < SchedulingAlgorithms.MAX_QUANTA) {
						sb.append(current.getId());

						float timeToFinish = current.getTimeToFinish() - 1;
						if (timeToFinish <= 0) {
							timeToFinish = 0;
						}

						current.setTimeToFinish(timeToFinish);

						if (timeToFinish == 0) {

							current.setTimeFinish(quantum);

							remove(current);

							d.add(current);

							current = null;
							break;
						}

						quantum++;

					}
				}

				quantum++;
			}
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

	private void add(Process process, int quantum) {
		ArrayList<Process> list = null;

		switch (process.getPriority()) {
		case 1:
			list = P1;
			break;

		case 2:
			list = P2;
			break;

		case 3:
			list = P3;
			break;

		case 4:
			list = P4;
			break;
		}

		if (list != null) {

			int listSize = list.size();
			if (listSize > 0) {
				Process prevProcess = list.get(listSize - 1);
				prevProcess.setNext(process);
			}

			list.add(process);

			process.setTimeWaiting(quantum);
		}
	}

	private Process select(Process currentProcess, int quantum) {
		Process process = null;

		int curPriority = 0;
		if (currentProcess != null) {
			curPriority = currentProcess.getPriority();

			currentProcess.setTimeWaiting(quantum);
		}

		if (aging) {
			int listSize = P2.size();
			for (int i = 0; i < listSize; i++) {
				Process p = P2.get(i);

				float waitingTime = p.getTimeWaiting();
				if (waitingTime >= 0 && quantum > waitingTime + 5) {
					P2.remove(p);
					i--;
					listSize--;

					p.setPriority(1);
					p.setTimeWaiting(quantum);
					P1.add(p);

				}
			}

			listSize = P3.size();
			for (int i = 0; i < listSize; i++) {
				Process p = P3.get(i);

				float waitingTime = p.getTimeWaiting();
				if (waitingTime >= 0 && quantum > waitingTime + 5) {
					P3.remove(p);
					i--;
					listSize--;

					p.setPriority(2);
					p.setTimeWaiting(quantum);
					P2.add(p);
				}
			}

			listSize = P4.size();
			for (int i = 0; i < listSize; i++) {
				Process p = P4.get(i);

				float waitingTime = p.getTimeWaiting();
				if (waitingTime >= 0 && quantum > waitingTime + 5) {
					P4.remove(p);
					i--;
					listSize--;

					p.setPriority(3);
					p.setTimeWaiting(quantum);
					P3.add(p);
				}
			}
		}

		for (int priority = 1; priority <= 4; priority++) {
			switch (priority) {
			case 1:
				if (priority == curPriority) {
					if (currentProcess != null) {
						process = currentProcess.getNext();
					}
				}

				if (process == null && P1.size() > 0) {
					process = P1.get(0);
				}
				break;

			case 2:
				if (priority == curPriority) {
					if (currentProcess != null) {
						process = currentProcess.getNext();
					}
				}

				if (process == null && P2.size() > 0) {
					process = P2.get(0);
				}
				break;

			case 3:
				if (priority == curPriority) {
					if (currentProcess != null) {
						process = currentProcess.getNext();
					}
				}

				if (process == null && P3.size() > 0) {
					process = P3.get(0);
				}
				break;

			case 4:
				if (priority == curPriority) {
					if (currentProcess != null) {
						process = currentProcess.getNext();
					}
				}

				if (process == null && P4.size() > 0) {
					process = P4.get(0);
				}
				break;
			}

			if (process != null) {
				process.setTimeWaiting(-1);

				return process;
			}
		}

		return null;
	}

	private void remove(Process process) {
		ArrayList<Process> list = null;

		switch (process.getPriority()) {
		case 1:
			list = P1;
			break;

		case 2:
			list = P2;
			break;

		case 3:
			list = P3;
			break;

		case 4:
			list = P4;
		}

		if (list != null) {

			int i = list.indexOf(process);
			if (i > 0) {
				Process nextProcess = null;
				if (i < list.size() - 1) {
					nextProcess = list.get(i + 1);
				}

				list.get(i - 1).setNext(nextProcess);
			}
			list.remove(process);
		}
	}

	public boolean isPreemptive() {
		return preemptive;
	}

	public void setPreemptive(boolean preemptive) {
		this.preemptive = preemptive;
	}

	public boolean isAging() {
		return aging;
	}

	public void setAging(boolean aging) {
		this.aging = aging;
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

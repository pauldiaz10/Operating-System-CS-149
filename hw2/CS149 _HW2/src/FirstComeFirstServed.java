import java.util.ArrayList;

public class FirstComeFirstServed implements Schedule {
	private float timeTurnaround;
	private float timeWaiting;
	private float timeResponse;
	private int throughputTotal;

	public String schedule(ArrayList<Process> a) {
		StringBuilder sb = new StringBuilder();
		int quantum = 0;
		int processes = 0;

		Process current = a.get(quantum);
		double remainingTime = current.getTotalExpectedRun();
		boolean running = true;
		boolean first = true;

		while (running) {
			if (current.getTimeArrival() > quantum) {
				quantum++;
				sb.append("- ");
			} else {
				if (first) {
					current.setTimeStart(quantum);
					first = false;
				}
				remainingTime = remainingTime - 1.0;
				if (remainingTime > 0 && quantum < SchedulingAlgorithms.MAX_QUANTA - 1 && first == false) {
					if ((quantum % 10) == 0) {
						sb.append("\n" + current.getId());
					} else {
						sb.append(current.getId());
					}
				} else if (remainingTime < 0 && quantum < SchedulingAlgorithms.MAX_QUANTA - 1) {
					if ((quantum % 10) == 0) {
						sb.append("\n" + current.getId());
					} else {
						sb.append(current.getId());
					}
					int nextQuantum = quantum + 1;
					current.setTimeFinish(nextQuantum);
					current = a.get(++processes);
					current.setTimeStart(nextQuantum);
					remainingTime = current.getTotalExpectedRun();
				} else if (remainingTime < 0 && quantum >= SchedulingAlgorithms.MAX_QUANTA - 1) {
					if ((quantum % 10) == 0) {
						sb.append("\n" + current.getId());
					} else {
						sb.append(current.getId());
					}
					current.setTimeFinish(quantum + 1);
					running = false;
				}
				quantum = quantum + 1;
			}
			throughputTotal = processes;
		}
		for (int i = 0; i < processes; i++) {
			timeTurnaround += a.get(i).getTurnaround();
			timeWaiting += a.get(i).getWaiting();
			timeResponse += a.get(i).getResponse();
		}
		timeTurnaround = timeTurnaround / processes;
		timeWaiting = timeWaiting / processes;
		timeResponse = timeResponse / processes;

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

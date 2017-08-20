import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class ShortestJobFirst implements Schedule {
	private float timeTurnaround;
	private float timeWaiting;
	private float timeResponse;
	private int throughputTotal;

	public String schedule(ArrayList<Process> a) {
		StringBuilder sb = new StringBuilder();

		ArrayList<Process> r = new ArrayList<>();
		ArrayList<Process> d = new ArrayList<>();

		Process next = null;
		Process current = null;
		int process = 0;
		int quantum = 0;

		Comparator<Process> finishTimeSorter = new Comparator<Process>() {

			@Override
			public int compare(Process p1, Process p2) {
				return Float.compare(p1.getTimeArrival(), p2.getTimeArrival());
			}

		};

		while (quantum < SchedulingAlgorithms.MAX_QUANTA) {

			boolean processes = false;
			if (next == null && process < a.size()) {
				next = a.get(process++);
			}
			while (next != null && next.getTimeArrival() <= quantum) {
				r.add(next);

				next.setTimeToFinish(next.getTotalExpectedRun());

				if (process < a.size() - 1) {
					next = a.get(process++);
				} else {
					next = null;
				}

				processes = true;
			}

			if (processes) {
				Collections.sort(r, finishTimeSorter);
			}

			if (r.size() > 0) {
				current = r.get(0);

				if (!current.isStarted()) {
					current.setStarted(true);
					current.setTimeStart(quantum);

				}
			}

			if (current != null) {
				sb.append(current.getId());

				float timeToFinish = current.getTimeToFinish() - 1;
				if (timeToFinish <= 0) {
					timeToFinish = 0;
				}

				current.setTimeToFinish(timeToFinish);

				if (timeToFinish == 0) {

					current.setTimeFinish(quantum);

					r.remove(current);

					d.add(current);

					current = null;
				} else {
					Collections.sort(r, finishTimeSorter);
				}
			} else {

			}

			quantum++;
		}
		int processes = d.size();
		for (int i = 0; i < processes; i++) {
			timeWaiting += d.get(i).getWaiting();
			timeTurnaround += d.get(i).getTurnaround();
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

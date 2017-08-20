import java.util.LinkedList;
import java.util.Random;

public class RandomPick implements ReplacementAlgorithm{

	String _name;
	
	public RandomPick(String name) {
		_name = name;
	}
	@Override
	public String getName() {
		return _name;
	}

	@Override
	public String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue) {
		
		Random rand = new Random(System.currentTimeMillis());
		int index;
		while (true) {
			index = rand.nextInt(replacementList.size());
			if (replacementList.get(index)._PageNumber != -1) {
				break;
			}
		}
		PageQueue.remove(replacementList.get(index));
		replacementList.set(index, new Page(-1, "."));
		
		return null;
	}

}

import java.util.LinkedList;

public interface ReplacementAlgorithm {
	String getName();
	String run(LinkedList<Page> replacementList, LinkedList<Page> PageQueue);
}

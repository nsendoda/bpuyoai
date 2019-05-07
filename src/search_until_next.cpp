#include "search_until_next.h"

SearchResultQueue SearchUntilNext::Excute(const State state,
                                          const Score GOAL_SCORE) {
  SearchResultQueue queue;
  for (PutIndex put_first = 0; put_first < PUTTYPE_PATTERN; put_first++) {
    Field first_field(state.field);
    PutType first_put(PutType::GetPutType(put_first));
    if (!Simulator::CanPut(first_put, first_field))
      continue;
    Frame first_frame =
        Simulator::Put(state.now_kumipuyo, &first_field, first_put);
    Chain first_chain = Simulator::Simulate(&first_field);
    first_chain.frame += first_frame;
    // Push
    if (first_chain.score >= GOAL_SCORE) {
      queue.Push(SearchResult({put_first}, first_chain));
      continue;
    }
    for (PutIndex put_second = 0; put_second < PUTTYPE_PATTERN; put_second++) {
      Field second_field(first_field);
      PutType second_put(PutType::GetPutType(put_second));
      if (!Simulator::CanPut(second_put, second_field))
        continue;
      Frame second_frame =
          Simulator::Put(state.next_kumipuyo, &second_field, second_put);
      Chain second_chain = Simulator::Simulate(&second_field);
      second_chain.frame += second_frame;

      // Push
      if (first_chain.score + second_chain.score >= GOAL_SCORE) {
        std::cout << "puts: " << put_first << ' ' << put_second << std::endl;
        std::cout << "first  "
                  << " putframe: " << first_frame
                  << " chainframe: " << first_chain.frame - first_frame
                  << std::endl;
        std::cout << "second "
                  << " putframe: " << second_frame
                  << " chainframe: " << second_chain.frame - second_frame
                  << std::endl;
        std::cout << "sum frame: " << first_chain.frame + second_chain.frame
                  << " score:" << first_chain.score + second_chain.score
                  << std::endl;
        queue.Push(SearchResult({put_first, put_second},
                                second_chain.Sum(first_chain)));
      }
    }
  }
  return queue;
}

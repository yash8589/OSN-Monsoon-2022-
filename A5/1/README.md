# Report for Question 1

- In this question the students are the threads and the number of wasing machines is the number of threads allowed inside the semaphore.
- We have used a struct called `time_args` that contains the student_id, the time of arrival, the patience of the student, and the time needed by the student to wash the clothes.
- The function `current_situation` is the function used by the threads and this contains the semaphore implementation

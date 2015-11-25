int test_teacher_availability(semesterData *sd){
   int a, b, c, d;
   int fitnessscore = 0;
   
   for (a = 0; a <= sd.numCourses; a++ ){
      
      for (b = 0; b <= sd.numLectures; b++){
         
         if (sd.courses[a].teachers.offTimes.day == sd.lectures[b].day){
            
            fitnessscore++;
            
         }
         
         if (sd.courses[a].teachers.offTimes.periods == sd.lectures[b].periods){
            
            fitnessscore++;
            
         }     
      }
   }
   return fitnessscore;
}

int is_teacher_available(int day, int period, teacher teachers)
{ 
   int a;
   
   for (a = 0; a < (sizeof(teachers.offTimes)/offTime); a++)
   {
      if (teachers.offTimes[a].day == day && (teachers.offTimes[a].periods[0] == period || teachers.offTimes[a].periods[1] == period))
      {  
         return 0;
      }
   }
   return 1;
}
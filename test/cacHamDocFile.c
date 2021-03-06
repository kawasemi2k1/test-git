
//de 20192 kip 2
void docFileDang1(Graph g, char *tenFile)
{
  FILE *f;
  int id;
  char ten[Max_Str];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed!\n");
      return;
    }
  int n,m;
  fscanf(f,"%d %d\n",&n,&m);
  int i;
  for(i=0;i<n;i++)
    {
      fscanf(f,"%d %s\n",&id,ten);
      __fpurge(stdin);
      addVertex(g,id,strdup(ten));
    }
  int id1,id2;
  double trongSo;
  for(i=0;i<m;i++)
    {
      fscanf(f,"%d %d %lf\n",&id1,&id2,&trongSo);
      __fpurge(stdin);
      addEdge(g,id1,id2,trongSo);
      addEdge(g,id2,id1,trongSo);
    }
}


// de 20192 kip1
int docFileDang2_1(char *tenFile, Graph g)
{
  FILE *f;
  char ten[Max_Str];
  int ip;
  int n;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  char str[Max_Str];
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",ten,&ip);
      strcat(ten," ");
      ten[strlen(ten)-1]=0;
      addVertex(g,ip,ten);
    }
  fclose(f);
  return n;
}

int docFileDang2_2(char *tenFile, Graph g)
{
  FILE *f;
  int ip,ipKhac;
  char str[Max_Str];
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  int n;
  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%d",&ip);
      fgets(str,Max_Str,f);
      str[strlen(str)-1]=0;
       strncpy(str,&str[1],strlen(str)-1);
       str[strlen(str)-1]=0;
       //take note
       // new la dang ip:ip1-ip2-ip3 thi thay s[2]="-";
      const char s[2]= " ";
      //chu y ben tren
      char *token;
      if(strlen(str)==4)
	addEdge(g,ip,atoi(str),1.0);
      else
	{
      token = strtok(str, s);
      while( token != NULL )
	{
	  printf("%s\n",token);
	  ipKhac=atoi(token);
	  printf("%d %d\n",ip,ipKhac);
	  addEdge(g,ip,ipKhac,1.0);
	  token = strtok(NULL, s);
	}
	}
    }
  fclose(f);
  return n;
}

//de 20192 kip1 tong quat
void docFileDang2TQ(char *tenFile, Graph g, int* soIP, int* soCanh)
{

   FILE *f;
  char ten[Max_Str];
  int ip,ipKhac;
  int n;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  fscanf(f,"%d\n",&n);
  char str[Max_Str];
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%s %d\n",ten,&ip);
      strcat(ten," ");
      ten[strlen(ten)-1]=0;
      addVertex(g,ip,ten);
    }
  *soIP=n;

  fscanf(f,"%d\n",&n);
  for(int i=0;i<n;i++)
    {
      fscanf(f,"%d",&ip);
      fgets(str,Max_Str,f);
      str[strlen(str)-1]=0;
       strncpy(str,&str[1],strlen(str)-1);
       str[strlen(str)-1]=0;
       //take note
       // new la dang ip:ip1-ip2-ip3 thi thay s[2]="-";
      const char s[2]= "-";
      //chu y ben tren
      
      char *token;
      if(strlen(str)==4)
	addEdge(g,ip,atoi(str),1.0);
      else
	{
      token = strtok(str, s);
      while( token != NULL )
	{
	  ipKhac=atoi(token);
	  addEdge(g,ip,ipKhac,1.0);
	  token = strtok(NULL, s);
	}
	}
    }
  fclose(f);
}

//de K55

//chu y de K55 la dang A1 A2 A3 ..., co the thay A bang 1 chu cai khac
// de nay la A1:A2-3 A3-6 nen chu y thay ":" = cac cai khac, "-" bang cac cai khac
int layID(char* str)
{
  return atoi(&str[1]);
}

int docFileDang3(char *tenFile, Graph g)
{
  FILE *f;
  char str[Max_String];
  char ten[Max_String];
  char ten2[Max_String];
  int soDinh=0;
  char *token, *token2;
  //note
  char s[2]=" ";
  char s2[2]= "-";
  //note
  int i;
  int j;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  while(!feof(f))
    {
      fgets(str,Max_String,f);
      if(strlen(str)==1) break;
      if(strlen(str)==0) break;
      str[strlen(str)-1]=0;
      soDinh++;
      for(i=0;i<strlen(str);i++)
	if(str[i]==':') break;  //note
      strncpy(ten,str,i);
      ten[i]=0;
      strcpy(str,&str[i+1]);
      if(ten[0]!='A') break; //note: co A
      addVertex(g,layID(ten),strdup(ten));
      if(strlen(str)==0)
	continue;
      
       token = strtok(str, s);
       while( token != NULL )
	 {
	   for(j=0;j<strlen(token);j++)
	     if(token[j]=='-')  //note
	       break;
	   strncpy(ten2,token,j);
	   ten2[j]=0;
	   strcpy(token,&token[j+1]);
	   addEdge(g,layID(ten2),layID(ten),atoi(token)); //ten2 cho den ten
	   token = strtok(NULL, s);
	 }
    }
   
  return soDinh-1;
  
}


//K55 nhung id la so ( dung cho data1.txt)

int docFileDang3MoRong(char *tenFile, Graph g)
{
  FILE *f;
  char str[Max_String];
  char ten[Max_String];
  char ten2[Max_String];
  int soDinh=0;
  char *token, *token2;
  int id,id2;
  //note
  char s[2]=" ";
  char s2[2]= "-";
  //note
  int i;
  int j;
  f=fopen(tenFile,"r");
  if(f==NULL)
    {
      printf("File open failed\n");
      exit(1);
    }
  while(!feof(f))
    {
      fgets(str,Max_String,f);
      if(strlen(str)==1) break;
      str[strlen(str)-1]=0;
      soDinh++;
      for(i=0;i<strlen(str);i++)
	if(str[i]==':') break;  //note
      strncpy(ten,str,i);
      ten[i]=0;
      id=atoi(ten);
      addVertex(g,id,strdup(ten)); //co the co hoac khong ( tuy de )
      if(strlen(str)==2)
	continue;
      strncpy(str,&str[i+1],strlen(str)-i);
      printf("%s\n",str);
      
      
       token = strtok(str, s);
       while( token != NULL )
	 {
	   for(j=0;j<strlen(token);j++)
	     if(token[j]=='-')  //note
	       break;
	   strncpy(ten2,token,j);
	   ten2[j]=0;
	   strcpy(token,&token[j+1]);
	   id2=atoi(ten2);
	   addEdge(g,id2,id,atoi(token)); // tuy vao de bai cho id2 cho den id hay gi 
	   token = strtok(NULL, s);
	 }
    }
   
  return soDinh-1;
  
}


// in graph
void printGraph(Graph g)
{
  JRB node,node2;
  JRB tree;
  printf("Vertieces:\n");
  jrb_traverse(node,g.vertices)
    printf("%d %s\n",jval_i(node->key),jval_s(node->val));
  printf("Edges:\n");
  jrb_traverse(node,g.edges)
    {
      tree=jval_v(node->val);
      printf("%d with ",jval_i(node->key));
      jrb_traverse(node2,tree)
	printf("%d(val = %lf)   ",jval_i(node2->key),jval_d(node2->val));
      printf("\n");
    }

}

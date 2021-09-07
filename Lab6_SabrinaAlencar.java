//classe da variavel compartilhada (soma de quantos numeros pares há no vetor) 
class Soma {
    //recurso compartilhado
    private int soma_pares;
    //construtor
    public Soma() { 
       this.soma_pares = 0; 
    }
  
    public synchronized void inc() { 
       this.soma_pares++; 
    }
    //operacao de leitura sobre o recurso compartilhado
    public synchronized int get() { 
       return this.soma_pares; 
    }
  }
  //cria uma classe que estende a classe Thread e implementa a tarefa de cada thread do programa 
  class ThreadPares extends Thread {
     //identificador da thread
     private int id;
     //objeto compartilhado com outras threads
     Soma s;
    
     //construtor
     public ThreadPares(int tid, Soma s) { 
        this.id = tid; 
        this.s = s;
     }
  
     //metodo main da thread
     public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        for (int i=(this.id); i<Lab6.N; i+=Lab6.NThreads) {
          //verificando se o numero é par
          if((Lab6.vetor[i])%2==0) this.s.inc(); 
        }
        System.out.println("Thread " + this.id + " terminou!"); 
     }
  }
  
  //--classe do metodo main
  class Lab6 {
     static final int N = 100; //tamanho do vetor
     static final int NThreads = 4; //Numero de threads
     static final int[] vetor = new int[N];
     
     public static void main (String[] args) {
        //vetor de threads e vetor de inteiros
        Thread[] threads = new Thread[NThreads];
      
        //inicializa vetor 
        for (int i=0; i<vetor.length; i++){
          vetor[i] = i;
        }
  
        //cria uma instancia do recurso compartilhado entre as threads
        Soma s = new Soma();
  
        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
           threads[i] = new ThreadPares(i, s);
        }
  
        //inicia as threads
        for (int i=0; i<threads.length; i++) {
           threads[i].start();
        }
  
        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
           try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
  
        //corretude
        if(s.get()==(N/2)) System.out.println("Saída correta");
        else System.out.println("Saída incorreta");
  
  
        System.out.println("Quantidade de pares = " + s.get()); 
     }
  
  }
// Implementa a padrao leitores/escritores
class Monitor {
	private int leitor, escritor;
	private int var = 0; // variável compartilhada entre as threads

	// Pega variavel compartilhada
	public synchronized int getvar() {
		return this.var;
	}

	// Seta a variavel compartilhada
	public synchronized void setvar(int valor) {
		this.var = valor;
	}

	// Construtor
	Monitor() {
		this.leitor = 0; // leitores lendo (0 ou mais)
		this.escritor = 0; // escritor escrevendo (0 ou 1)
	}

	// Entrada para leitores
	public synchronized void EntradaLeitor(int id) {
		try {
			while (this.escritor > 0) {
				System.out.println("le.leitorBloqueado(" + id + ")");
				wait(); // bloqueia pela condicao logica da aplicacao
			}
			this.leitor++; // mais um leitor lendo
			System.out.println("le.leitorLendo(" + id + ")");
		} catch (InterruptedException e) {
			System.err.println(e);
		}
	}

	// Saida para leitores
	public synchronized void SaidaLeitor(int id) {
		this.leitor--; // registra que um leitor saiu
		if (this.leitor == 0)
			this.notify(); // libera escritor (caso exista escritor bloqueado)
		System.out.println("le.leitorSaindo(" + id + ")");
	}

	// Entrada para escritores
	public synchronized void EntradaEscritor(int id) {
		try {
			while ((this.leitor > 0) || (this.escritor > 0)) {
				System.out.println("le.escritorBloqueado(" + id + ")");
				wait(); // bloqueia pela condicao logica da aplicacao
			}
			this.escritor++; // tem um escritor escrevendo
			System.out.println("le.escritorEscrevendo(" + id + ")");
		} catch (InterruptedException e) {
			System.err.println(e);
		}
	}

	// Saida para escritores
	public synchronized void SaidaEscritor(int id) {
		this.escritor--; // escritor saiu
		notifyAll(); // libera leitores e escritores 
		System.out.println("le.escritorSaindo(" + id + ")");
	}
}

// Thread leitora
class Leitora extends Thread {
	int id; // identificador da thread
	int tempo; 
	Monitor monitor; 

	// Construtor
	Leitora(int id, int tempo, Monitor m) {
		this.id = id;
		this.tempo = tempo;
		this.monitor = m;
	}

	// tarefa da thread => verifica se é primo
	public void primo(int valor) {
		if (valor == 0) {
			System.out.println("------Valor: " + valor + " nao eh primo------");
			return;
		}

		for (int i = 2; i < valor; i++) {
			if (valor % i == 0) {
				System.out.println("------Valor: " + valor + " nao eh primo------");
				return;
			}
		}

		System.out.println("------Valor: " + valor + " eh primo------");
	}

	// Método executado pela thread
	@Override
	public void run() {
		try {
			for (int i = 0; i < Main.limite; i++) {
				this.monitor.EntradaLeitor(this.id);
				primo(this.monitor.getvar());
				//System.out.println("Thread leitora " + this.id + " leu");
				this.monitor.SaidaLeitor(this.id);
				sleep(this.tempo);
			}
		} catch (InterruptedException e) {
			System.err.println(e);
			return;
		}
	}
}

// Thread escritora
class Escritora extends Thread {
	int id; // identificador da thread
	int tempo; 
	Monitor monitor; 

	// Construtor
	Escritora(int id, int tempo, Monitor m) {
		this.id = id;
		this.tempo = tempo;
		this.monitor = m;
	}

	// Método executado pela thread
	public void run() {
		try {
			for (int i = 0; i < Main.limite; i++) {
				this.monitor.EntradaEscritor(this.id);
				this.monitor.setvar(this.id);
				//System.out.println("Thread escritora " + this.id + " escreveu: " + this.id);
				this.monitor.SaidaEscritor(this.id);
				sleep(this.tempo);
			}
		} catch (InterruptedException e) {
			System.err.println(e);
			return;
		}
	}
}

// Thread leitora/escritora
class LeitEscr extends Thread {
	int id; // identificador da thread
	int tempo; 
	Monitor monitor; // objeto monitor para coordenar a lógica de execução das threads

	// Construtor
	LeitEscr(int id, int tempo, Monitor m) {
		this.id = id;
		this.tempo = tempo;
		this.monitor = m;
	}

	// tarefa da thread => verifica se número eh par ou impar
	public void par(int valor) {
		if (valor % 2 == 0) {
			System.out.println("------Valor: " + valor + " eh par------");
			return;
		}
		System.out.println("------Valor: " + valor + " eh impar------");
	}

	// Método executado pela thread
	@Override
	public void run() {
		try {
			for (int i = 0; i < Main.limite; i++) {
				
        // parte da leitura 
				this.monitor.EntradaLeitor(this.id);
				par(this.monitor.getvar());
				//System.out.println("Thread leitora/escritora " + this.id + " leu");
				this.monitor.SaidaLeitor(this.id);
				
        // parte da escrita 
				this.monitor.EntradaEscritor(this.id);
				this.monitor.setvar(2 * this.monitor.getvar());
				//System.out.println("Thread leitora/escritora " + this.id + " escreveu: " + this.monitor.getvar());
				this.monitor.SaidaEscritor(this.id);

				sleep(this.tempo);
			}
		} catch (InterruptedException e) {
			System.err.println(e);
			return;
		}
	}
}

// Classe principal
class Lab7 {

	private static final int L = 3; // número de threads leitoras
	private static final int E = 5; // número de threads escritoras
	private static final int LE = 2; // número de threads leitoras/escritoras
  public static final int limite = 5; // total de vezes que as threads executam 

	public static void main(String[] args) {
		Monitor monitor = new Monitor(); // Monitor - Compartilhado entre leitores e escritores
		Leitora[] Leitoras = new Leitora[L]; // Threads leitoras
		Escritora[] Escritoras = new Escritora[E]; // Threads escritoras
		LeitEscr[] LeitorasAndEscritoras = new LeitEscr[LE]; // Threads leitoras/escritoras

		// Inicia o log de saida
		System.out.println("import verificaLE");
		System.out.println("le = verificaLE.LE()");

		// Cria leitoras
		for (int i = 0; i < L; i++) {
			Leitoras[i] = new Leitora(i + 1, 1000, monitor);
			Leitoras[i].start();
		}

		// Cria escritoras
		for (int i = 0; i < E; i++) {
			Escritoras[i] = new Escritora(i + 1, 1000, monitor);
			Escritoras[i].start();
		}

		// Cria os leitores/escritores
		for (int i = 0; i < LE; i++) {
			LeitorasAndEscritoras[i] = new LeitEscr(i + 1, 1000, monitor);
			LeitorasAndEscritoras[i].start();
		}
	}
}
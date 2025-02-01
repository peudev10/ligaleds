# Controle de LEDs com Botão - Raspberry Pi Pico

## Descrição
Este projeto utiliza a Raspberry Pi Pico para controlar três LEDs (vermelho, amarelo e verde) através de um botão. Quando pressionado, o botão acende todos os LEDs simultaneamente e inicia um desligamento progressivo, com um intervalo de 3 segundos entre cada LED apagado. O sistema também implementa debounce por software para evitar leituras falsas do botão.

## Funcionamento

1. **Configuração dos LEDs:** Os pinos dos LEDs são inicializados como saída.
2. **Configuração do botão:** O botão é configurado como entrada com pull-up interno e uma interrupção na borda de descida.
3. **Sequência de ativação:**
   - Ao pressionar o botão, todos os LEDs acendem simultaneamente.
   - Após 3 segundos, o LED vermelho apaga.
   - Após mais 3 segundos, o LED amarelo apaga.
   - Após mais 3 segundos, o LED verde apaga, permitindo uma nova ativação.
4. **Debounce:** Implementado por software para evitar leituras incorretas do botão.
5. **Loop principal:** O programa mantém a Raspberry Pi Pico ativa enquanto aguarda interrupções do botão.




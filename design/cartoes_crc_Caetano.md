### Classe: TemplateReceita
| Responsabilidades | Colaborações |
| :---------------- | :----------- |
| Armazenar o nome do template (ex.: Pizza, Risoto, hamburguei) e sua descrição | Receita |
| Conhecer as listas de ingredientes obrigatórios e proibidos | Ingrediente |
| Validar se uma Receita candidata atende às regras do template | ---- |
| Gerar uma nova Receita pre preenchida a partir do template | ---- |
| Permitir extensão por herança para novos tipos de pratos, como TemplateSobremesa, ou TemplateEntrada .. | ---- |



### Classe: Ingrediente
| Responsabilidades | Colaborações |
| :---------------- | :----------- |
| Armazenar nome, quantidade e unidade de medida | Receita |
| Conhecer o tipo/categoria do ingrediente, sólido, líquido, etc.| TemplateReceita |
| Validar a quantidade informada, se é não-negativa e compatível com a unidade | ---- |
| Comparar igualdade com outro ingrediente para a lista de compras | ---- |
| Imprimir-se de forma polimórfica conforme o tipo de ingrediente | ---- |

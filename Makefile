OBJ = textToBinary healthAssessment
all: $(OBJ)

textToBinary: textToBinary.c
	gcc -Wall -o textToBinary textToBinary.c

healthAssessment:healthAssessment.c
	gcc -Wall -o healthAssessment healthAssessment.c

clean:
	rm -f *.bin $(OBJ)

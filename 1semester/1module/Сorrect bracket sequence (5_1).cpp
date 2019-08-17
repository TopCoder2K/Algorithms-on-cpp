/* Решение всех задач данного раздела предполагает использование стека. Способ реализации стека может быть любым (список/динамический массив).
5_1. Скобочная последовательность.
Дан фрагмент последовательности скобок, состоящей из символов (){}[].
Требуется определить, возможно ли продолжить фрагмент в обе стороны, получив корректную последовательность.
Длина исходной последовательности ≤ 800000.
Формат входных данных. Строка, содержащая символы (){}[].
Формат выходных данных. Если возможно - вывести минимальную корректную последовательность, иначе - напечатать "IMPOSSIBLE". */

#include <iostream>
#include <stack>
#include <deque>

void CloseLeftScopes(std::stack<char>& scopes, std::deque<char>& answer)
{
	while (!scopes.empty() && (scopes.top() != '}' && scopes.top() != ')' && scopes.top() != ']')) {
		if (scopes.top() == '[') {
			answer.push_back(']');
			scopes.pop();
		}
		else if (scopes.top() == '(') {
			answer.push_back(')');
			scopes.pop();
		}
		else if (scopes.top() == '{') {
			answer.push_back('}');
			scopes.pop();
		}
	}
}

std::stack<char> StackReverse(std::stack<char>& scopes) {
	std::stack<char> reversed_stack;
	while (!scopes.empty()) {
		reversed_stack.push(scopes.top());
		scopes.pop();
	}

	return reversed_stack;
}

void CloseRightScopes(std::stack<char>& scopes, std::deque<char>& answer) {
	while (!scopes.empty() && (scopes.top() != '{' && scopes.top() != '(' && scopes.top() != '[')) {
		if (scopes.top() == ']') {
			answer.push_front('[');
			scopes.pop();
		}
		else if (scopes.top() == ')') {
			answer.push_front('(');
			scopes.pop();
		}
		else if (scopes.top() == '}') {
			answer.push_front('{');
			scopes.pop();
		}
	}
}

int main() {
	char s = 0;
	std::stack<char> scopes;
	std::deque<char> answer;

	while (std::cin >> s) {
		// Manual testing requires a trailing character.
		if (s == 'f')
			break;

		answer.push_back(s);
		// Removing pairs of matching brackets.
		if (!scopes.empty()) {
			if ((scopes.top() == '[' && s == ']') || (scopes.top() == '{' && s == '}') || (scopes.top() == '(' && s == ')')) {
				scopes.pop();
				continue;
			}
		}

		scopes.push(s);
	}

	CloseLeftScopes(scopes, answer);
	scopes = StackReverse(scopes);
	CloseRightScopes(scopes, answer);

	if (scopes.empty()) {
		while (!answer.empty()) {
			std::cout << answer.front();
			answer.pop_front();
		}
	}
	else {
		std::cout << "IMPOSSIBLE";
	}

	return 0;
}

/*
in
{}[[[[{}[]f
out
{}[[[[{}[]]]]]
----------------
in
{][[[[{}[]f
out
IMPOSSIBLE
 */
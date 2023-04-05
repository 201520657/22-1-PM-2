#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
using namespace std;

int n, m, k, c, drug_max, res;
int dr_tree[] = { -1, 0, 1, 0 };
int dc_tree[] = { 0, 1, 0, -1 };
int dr_drug[] = { -1, 1, 1, -1 };
int dc_drug[] = { 1, 1, -1, -1 };
int tree_mp[20][20], drug_mp[20][20];

void tree_grow() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree_mp[i][j] > 0) {
				int cnt = 0;

				for (int a = 0; a < 4; a++) {
					int nr = i + dr_tree[a];
					int nc = j + dc_tree[a];

					if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
						continue;
					}

					if (tree_mp[nr][nc] > 0) {
						cnt++;
					}
				}

				tree_mp[i][j] += cnt;
			}
		}
	}
}

void tree_spread() {
	int temp[20][20] = { 0, };

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tree_mp[i][j] > 0) {
				int cnt = 0;

				for (int a = 0; a < 4; a++) {
					int nr = i + dr_tree[a];
					int nc = j + dc_tree[a];

					if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
						continue;
					}

					if (tree_mp[nr][nc] == 0 && drug_mp[nr][nc] == 0) {
						cnt++;
					}
				}
				
				int divide;

				if (cnt == 0) {
					divide = 0;
				}

				else {
					divide = tree_mp[i][j] / cnt;
				}

				for (int a = 0; a < 4; a++) {
					int nr = i + dr_tree[a];
					int nc = j + dc_tree[a];

					if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
						continue;
					}

					if (tree_mp[nr][nc] == 0 && drug_mp[nr][nc] == 0) {
						temp[nr][nc] += divide;
					}
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (temp[i][j] != 0) {
				tree_mp[i][j] = temp[i][j];
			}
		}
	}
}

void pre_drug() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (drug_mp[i][j] > 0) {
				drug_mp[i][j]--;
			}
		}
	}
}

void drug() {
	drug_max = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int cnt = 0;

			if (tree_mp[i][j] == 0) {
				continue;
			}

			else {
				int check[20][20] = { 0, };

				for (int a = 0; a < 4; a++) {
					for (int b = 0; b <= k; b++) {
						int nr = i + dr_drug[a] * b;
						int nc = j + dc_drug[a] * b;

						if (nr < 0 || nr >= n || nc < 0 || nc >= n || tree_mp[nr][nc] == -1 || tree_mp[nr][nc] == 0) {
							break;
						}

						else {
							if (tree_mp[nr][nc] > 0 && check[nr][nc] == 0) {
								check[nr][nc] = 1;
								cnt += tree_mp[nr][nc];
							}
						}
					}
				}
			}

			drug_max = max(drug_max, cnt);
		}
	}

	int max_r, max_c;
	bool flag = false;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int cnt = 0;

			if (tree_mp[i][j] == 0) {
				continue;
			}

			else {
				int check[20][20] = { 0, };

				for (int a = 0; a < 4; a++) {
					for (int b = 0; b <= k; b++) {
						int nr = i + dr_drug[a] * b;
						int nc = j + dc_drug[a] * b;

						if (nr < 0 || nr >= n || nc < 0 || nc >= n || tree_mp[nr][nc] == -1 || tree_mp[nr][nc] == 0) {
							break;
						}

						else {
							if (tree_mp[nr][nc] > 0 && check[nr][nc] == 0) {
								check[nr][nc] = 1;
								cnt += tree_mp[nr][nc];
							}
						}
					}
				}

				if (cnt == drug_max) {
					max_r = i;
					max_c = j;
					flag = true;
					break;
				}
			}
		}

		pre_drug();

		if (flag == true) {
			int check[20][20] = { 0, };

			for (int a = 0; a < 4; a++) {
				for (int b = 0; b <= k; b++) {
					int nr = max_r + dr_drug[a] * b;
					int nc = max_c + dc_drug[a] * b;

					if (nr < 0 || nr >= n || nc < 0 || nc >= n || check[nr][nc] == 1) {
						// || tree_mp[nr][nc] == -1
						break;
					}

					if (check[nr][nc] == 0 && (tree_mp[nr][nc] == -1 || tree_mp[nr][nc] == 0)) {
						//check[nr][nc] == 0 && 
						tree_mp[nr][nc] = 0;
						drug_mp[nr][nc] = c;
						check[nr][nc] = 1;
						break;
					}

					else {
						if (tree_mp[nr][nc] > 0 && check[nr][nc] == 0) {
							tree_mp[nr][nc] = 0;
							drug_mp[nr][nc] = c;
							check[nr][nc] = 1;
						}
					}
				}
			}

			res += drug_max;
			break;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("input.txt", "rt", stdin);

	cin >> n >> m >> k >> c;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> tree_mp[i][j];
		}
	}

	for (int z = 0; z < m; z++) {
		tree_grow();

		tree_spread();

		
		drug();

		/*for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << tree_mp[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n\n";

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << drug_mp[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "\n\n";*/
	}

	cout << res << "\n";
	return 0;
}
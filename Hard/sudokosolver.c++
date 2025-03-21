class SubValidator {

    protected:
        int size;
        vector<vector<bool>> valuePresenceChecker;
    
    public:
        
        SubValidator(int n) {
            size = n;
            valuePresenceChecker.resize(n);
            for (int i=0; i<n; i++) valuePresenceChecker[i].resize(n, false);
        }
        virtual ~SubValidator() {}
        virtual bool validate(int row, int col, int val)=0;
        virtual void fillCell(int row, int col, int val, bool flag)=0;
    
    };
    
    class ColumnValidator : public SubValidator {  
        
    public:
        
        ColumnValidator(int n) : SubValidator(n) {}
        
        bool validate(int row, int col, int val) override {
            if (val<1 || val>size || valuePresenceChecker[col][val-1]) return false;
            return true;
        }
    
        void fillCell(int row, int col, int val, bool flag) override {
            valuePresenceChecker[col][val-1] = flag;
        }
    
    };
    
    class RowValidator : public SubValidator {
        
    public:
        
        RowValidator(int n) : SubValidator(n) {}
        
        bool validate(int row, int col, int val) override {
            if (val<1 || val>size || valuePresenceChecker[row][val-1]) return false;
            return true;
        }
    
        void fillCell(int row, int col, int val, bool flag) override {
            valuePresenceChecker[row][val-1] = flag;
        }
    
    };
    
    class SubBoxValidator : public SubValidator {
        
        int sqrtSize;
    
    public:
        
        SubBoxValidator(int n) : SubValidator(n) {
            for (int i=1; i*i<=n; i++) sqrtSize = i;
        }
        
        int getBoxIdx (int row, int col) {
            return (row/sqrtSize)*sqrtSize + (col/sqrtSize);
        }
    
        bool validate(int row, int col, int val) override {
            int boxIdentity = getBoxIdx(row, col);
            if (val<1 || val>size || valuePresenceChecker[boxIdentity][val-1]) return false;
            return true;
        }
    
        void fillCell(int row, int col, int val, bool flag) override {
            int boxIdentity = getBoxIdx(row, col);
            valuePresenceChecker[boxIdentity][val-1] = flag;
        }
    
    };
    
    class Validator {
    
        vector<unique_ptr<SubValidator>> subValidations;
    
    public:
        
        Validator(int n) {
            subValidations.push_back(make_unique<ColumnValidator>(n));
            subValidations.push_back(make_unique<RowValidator>(n));
            subValidations.push_back(make_unique<SubBoxValidator>(n));
        }
    
        bool validate(int row, int col, int val) {
            bool ans = true;
            for (auto& subValidation : subValidations) ans &= subValidation->validate(row, col, val);
            return ans;
        }
    
        void fillCell(int row, int col, int val, bool flag) {
            for (auto& subValidation : subValidations) subValidation->fillCell(row, col, val, flag);
        }
    
    };
    
    
    class Solution {
    
        void fillSudoku(int currRow, int currCol, int val, shared_ptr<Validator>& validator, vector<vector<char>>& board, bool flag) {
            board[currRow][currCol] = flag ? '0'+val : '.';
            validator->fillCell(currRow, currCol, val, flag);
        }
    
        bool solveSudoku(int currRow, int currCol, int boardSize, shared_ptr<Validator>& validator, vector<vector<char>>& board) {
            if (currRow == boardSize) return true;
            if (currCol == boardSize) return solveSudoku(currRow+1, 0, boardSize, validator, board);
            if (board[currRow][currCol] != '.') return solveSudoku(currRow, currCol+1, boardSize, validator, board);
    
            for (int possVal = 1; possVal <= boardSize; possVal++) {
                bool isCurrValPoss = validator->validate(currRow, currCol, possVal);
                if (!isCurrValPoss) continue;
                fillSudoku(currRow, currCol, possVal, validator, board, true);
                bool isSolved = solveSudoku(currRow, currCol+1, boardSize, validator, board);
                if (isSolved) return true;
                fillSudoku(currRow, currCol, possVal, validator, board, false);
            }
            
            return false;
        }
    
    public:
        
        void solveSudoku(vector<vector<char>>& board) {
            
            int n = board.size();
            if (n==0) return;
            if (board[0].size() != n) return;
            shared_ptr<Validator> sudokuValidator = make_shared<Validator>(n);
            
            for (int i=0; i<n; i++)
                for (int j=0; j<n; j++)
                    if (board[i][j] == '.') continue;
                    else if (!sudokuValidator->validate(i,j,board[i][j]-'0')) return;
                    else sudokuValidator->fillCell(i,j,board[i][j]-'0',true);
            
            bool isSolved = solveSudoku(0, 0, n, sudokuValidator, board);
    
        }
    };                        
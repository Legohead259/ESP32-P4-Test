import os

def parse_gitignore(path=".gitignore"):
    blacklist = []
    if not os.path.exists(path):
        return blacklist

    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "/" in line or "*" in line:
                continue
            blacklist.append(line.rstrip("/"))

    return blacklist

def generate_tree(start_path='.', prefix='', level=0, max_depth=2, graylist=None, blacklist=None):
    if graylist is None:
        graylist = []
    if blacklist is None:
        blacklist = []

    tree = ""
    if level >= max_depth:
        return tree

    entries = sorted(os.listdir(start_path))
    entries = [e for e in entries if not e.startswith('__pycache__')]
    last_index = len(entries) - 1

    for i, entry in enumerate(entries):
        if entry not in blacklist:
            path = os.path.join(start_path, entry)
            connector = '└── ' if i == last_index else '├── '
            entry_str = entry + '/' if os.path.isdir(path) else entry
            tree += prefix + connector + entry_str + '\n'

            if os.path.isdir(path) and entry not in graylist:
                extension = '    ' if i == last_index else '│   '
                tree += generate_tree(path, prefix + extension, level + 1, max_depth, graylist, blacklist)

    return tree

def define_env(env):
    @env.macro
    def build_tree(path='.', max_depth=2, graylist=None, blacklist=None):
        """
        Usage: {{ build_tree(path='.', max_depth=2, graylist=['lib'], blacklist=['.git']) }}
        If blacklist is None, it will auto-parse `.gitignore`.
        """
        if blacklist is None:
            blacklist = parse_gitignore(path+"/.gitignore")
        if graylist is None:
            graylist = []

        root = os.path.basename(os.path.abspath(path))
        tree = root + '/\n' + generate_tree(path, max_depth=max_depth, graylist=graylist, blacklist=blacklist)
        return f"```text\n{tree}```"

if __name__ == "__main__":
    blacklist = parse_gitignore(".gitignore")
    graylist = ["test", "include"]  # Add graylist values here if needed

    root_dir = os.path.basename(os.getcwd())
    tree_output = root_dir + "/\n" + generate_tree(".", graylist=graylist, blacklist=blacklist)

    print(tree_output)
#!/usr/bin/python

from __future__ import print_function
import argparse
import os
import plistlib


class Diagnostics:
    def __init__(self, value):
        self.level = value.get('level', 'note')
        self.message = value.get('message', '')
        self.file = value.get('filename', '')
        self.line = value.get('line', 0)
        self.column = value.get('column', 0)
        # Trim the Root path away from file path.
        if self.file.find('SDKContentRoot') != -1:
            self.file = self.file.split('SDKContentRoot')[-1]


    def __repr__(self):
        if len(self.file) == 0:
            return '{lvl}: {msg}'.format(lvl=self.level, msg=self.message)
        return '{file}:{line}:{col} {lvl}: {msg}'.format(file=self.file, line=self.line, col=self.column,
                                                         lvl=self.level, msg=self.message)


class DiagGroup:
    def __init__(self, diff):
        self.diff = diff
        self.reasons = []

    def addReason(self, reason):
        self.reasons.append(reason)

    def print(self):
        print(self.diff)
        for r in self.reasons:
            print(r)
        print('')


def find_all_diag_file(path,update_layout=False):
    # Filter out localization projects
    projects = filter(lambda x: '_Loc_' not in x, os.listdir(path))

    diag_file = []
    missing = []
    for p in projects:
        project_path = os.path.join(path, p)
        if update_layout:
            if not os.path.exists(os.path.join(project_path, 'SDKContentRoot')):
                continue
            project_path = os.path.join(project_path, '.BuildData', 'SDKDB')
        if not os.path.exists(project_path):
            missing.append(p)
            continue
        diag_path = os.path.join(project_path, 'diag.plist')
        if os.path.exists(diag_path):
            diag_file.append(diag_path)
        else:
            missing.append(p)

    return diag_file, missing


def extract_diagnostics(path):
    with open(path, 'r') as f:
        data = f.read()
        # Complete the plist (the log itself is just the chunks).
        data = """\
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" \
                       "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<array>
%s
</array>
</plist>""" % data

        diags = plistlib.readPlistFromString(data)

    for file_diag in diags:
        name = file_diag.get('main-file', '')
        if not os.path.basename(name).startswith('tapi'):
            # All the files are expected to has main file name begin with tapi.
            continue

        # 7000 is just a large number to ignore all the warnings from clang and only leave the ones from tapi.
        # Number might need to be adjusted in the future.
        selected_diags = [Diagnostics(d) for d in file_diag.get('diagnostics') if d.get('ID') > 7000]
        return selected_diags

    return []


def main():
    parser = argparse.ArgumentParser(description='Extract diagnostics from SDKDB generation')

    parser.add_argument('-u', '--update-layout', action='store_true', help='use OS Update BuildRecord layout')
    parser.add_argument('records', help='Path to records')

    args = parser.parse_args()

    files, missing = find_all_diag_file(args.records, args.update_layout)

    results = sum([extract_diagnostics(p) for p in files], [])

    diags = []
    for d in results:
        if d.level == 'warning' or d.level == 'error':
            diags.append(DiagGroup(d))
        else:
            diags[-1].addReason(d)

    print("Found {} projects missing diagnostic files: {}".format(len(missing), missing))
    print("Found {} errors/warnings:".format(len(diags)))
    for d in diags:
        d.print()


if __name__ == '__main__':
    main()

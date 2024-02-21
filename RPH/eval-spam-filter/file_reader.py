from email import policy, parser


def read_text_file(path: str) -> str:
    file = open(path, 'r', encoding='utf-8')
    data = file.read()
    file.close()
    return data


def read_email_file(path: str):
    file = open(path, 'rb')
    email = parser.BytesParser(policy=policy.default).parse(file)
    data = {}
    data['body'] = email.get_payload()
    data['subject'] = email.get('subject')
    if email.is_multipart():
        data['body'] = data['body'][0]
    file.close()
    return data
